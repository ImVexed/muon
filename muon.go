package muon

import (
	"encoding/json"
	"errors"
	"net"
	"net/http"
	"reflect"
	"unsafe"

	. "github.com/ImVexed/muon/ultralight"
)

// Window represents a single Ultralight instance
type Window struct {
	wnd       ULWindow
	ov        ULOverlay
	view      ULView
	app       ULApp
	handler   http.Handler
	cfg       *Config
	callbacks map[string]*ipf
}

type ipf struct {
	Function   reflect.Value
	ParamTypes []reflect.Type
}

// Config contains configurable controls for the Ultralight engine
type Config struct {
	Title  string
	Height uint32
	Width  uint32
	X      int32
	Y      int32

	Resizeable  bool
	Borderless  bool
	Tilted      bool
	Maximizable bool
}

// New creates a Ultralight Window
func New(cfg *Config, handler http.Handler) *Window {
	w := &Window{
		cfg:       cfg,
		handler:   handler,
		callbacks: make(map[string]*ipf),
	}

	ufg := UlCreateConfig()
	std := UlCreateSettings()
	w.app = UlCreateApp(std, ufg)
	mm := UlAppGetMainMonitor(w.app)

	var hint uint32

	if cfg.Resizeable {
		hint |= 4
	}

	if cfg.Borderless {
		hint |= 1
	}

	if cfg.Tilted {
		hint |= 2
	}

	if cfg.Maximizable {
		hint |= 8
	}

	w.wnd = UlCreateWindow(mm, w.cfg.Height, w.cfg.Width, false, hint)

	UlWindowSetTitle(w.wnd, w.cfg.Title)
	UlAppSetWindow(w.app, w.wnd)

	w.ov = UlCreateOverlay(w.wnd, w.cfg.Height, w.cfg.Width, w.cfg.X, w.cfg.Y)

	UlWindowSetResizeCallback(w.wnd, resizeCallback(w.ov), nil)

	w.view = UlOverlayGetView(w.ov)

	return w
}

// Start sets up the Ultralight runtime and begins showing the Window
func (w *Window) Start() error {

	addr, err := serveHandler(w.handler)

	if err != nil {
		return err
	}

	url := UlCreateString(addr)
	defer UlDestroyString(url)

	UlViewLoadURL(w.view, url)

	UlAppRun(w.app)

	return nil
}

var registerCount int

// Bind registers the given function to the given name in the Window's JS global object
func (w *Window) Bind(name string, function interface{}) {
	f := &ipf{
		Function: reflect.ValueOf(function),
	}

	t := f.Function.Type()

	f.ParamTypes = make([]reflect.Type, t.NumIn())

	for i := 0; i < t.NumIn(); i++ {
		f.ParamTypes[i] = t.In(i)
	}

	if t.NumOut() > 1 {
		panic("Too many return values!")
	}

	w.callbacks[name] = f

	w.addFunction(name)
}

// Eval evaluates a given JavaScript string in the given Window view. `ret` is necessary for JSON serialization if an object is returned.
func (w *Window) Eval(js string, ret reflect.Type) (interface{}, error) {
	us := UlCreateString(js)
	defer UlDestroyString(us)

	ref := UlViewEvaluateScript(w.view, us)
	ctx := UlViewGetJSContext(w.view)

	val, err := fromJSValue(ctx, ref, ret)

	if err != nil {
		return nil, err
	}

	return val.Interface(), nil
}

// Resize changes the given Window's size
func (w *Window) Resize(width int, height int) {
	UlOverlayResize(w.ov, uint32(width), uint32(height))
}

// Move sets the Window's position to the given coordinates
func (w *Window) Move(x int, y int) {
	UlOverlayMoveTo(w.ov, int32(x), int32(y))
}

func (w *Window) ipcCallback(ctx JSContextRef, functin JSObjectRef, thisObject JSObjectRef, argumentCount uint, arguments []JSValueRef, exception []JSValueRef) JSValueRef {
	jsName := JSStringCreateWithUTF8CString("name")
	defer JSStringRelease(jsName)

	prop := JSObjectGetProperty(ctx, functin, jsName, nil)
	jsProp := JSValueToStringCopy(ctx, prop, nil)
	defer JSStringRelease(jsProp)

	name := fromJSString(jsProp)

	f, ok := w.callbacks[name]

	if !ok {
		return JSValueMakeNull(ctx)
	}

	params := make([]reflect.Value, argumentCount)

	for i := uint(0); i < argumentCount; i++ {
		val, err := fromJSValue(ctx, arguments[i], f.ParamTypes[i])

		if err != nil {
			panic(err)
		}

		params[i] = val
	}

	val := f.Function.Call(params)

	if len(val) > 1 {
		panic("Javascript does not support more than 1 return value!")
	}

	if len(val) == 0 {
		return JSValueMakeNull(ctx)
	}

	return toJSValue(ctx, val[0])
}

func fromJSValue(ctx JSContextRef, value JSValueRef, rtype reflect.Type) (reflect.Value, error) {
	if rtype == nil {
		rtype = reflect.TypeOf(struct{}{})
	}

	var rv reflect.Value
	var err error

	if JSValueIsArray(ctx, value) {
		l := JSStringCreateWithUTF8CString("length")
		defer JSStringRelease(l)

		obj := *(*JSObjectRef)(unsafe.Pointer(&value))

		prop := JSObjectGetProperty(ctx, obj, l, nil)
		length := int(JSValueToNumber(ctx, prop, nil))

		if rtype.Kind() != reflect.Slice {
			return reflect.Zero(rtype), errors.New("JS return is of type Array while Go type target is not")
		}

		values := reflect.MakeSlice(rtype, length, length)

		for i := 0; i < length; i++ {
			ref := JSObjectGetPropertyAtIndex(ctx, obj, uint32(i), nil)

			val, err := fromJSValue(ctx, ref, rtype.Elem())

			if err != nil {
				return reflect.Zero(rtype), err
			}

			values.Index(i).Set(val)
		}

		return values, nil
	}

	switch JSValueGetType(ctx, value) {
	case KJSTypeBoolean:
		rv = reflect.ValueOf(JSValueToBoolean(ctx, value))
	case KJSTypeNumber:
		rv = reflect.ValueOf(JSValueToNumber(ctx, value, nil))
	case KJSTypeString:
		ref := JSValueToStringCopy(ctx, value, nil)
		rv = reflect.ValueOf(fromJSString(ref))
		JSStringRelease(ref)
	case KJSTypeObject:
		ref := JSValueCreateJSONString(ctx, value, 0, nil)
		obj := reflect.New(rtype).Interface()

		if err = json.Unmarshal([]byte(fromJSString(ref)), &obj); err == nil {
			rv = reflect.Indirect(reflect.ValueOf(obj))
		}

		JSStringRelease(ref)
	case KJSTypeUndefined, KJSTypeNull:
		rv = reflect.Zero(rtype)
	}

	return rv, err
}

func fromJSString(str JSStringRef) string {
	len := JSStringGetMaximumUTF8CStringSize(str)
	data := make([]byte, len)
	written := JSStringGetUTF8CString(str, data, len)

	return string(data[:written-1])
}

func toJSValue(ctx JSContextRef, value reflect.Value) JSValueRef {
	var jsv JSValueRef
	var err error

	switch value.Kind() {
	case reflect.Float64:
		jsv = JSValueMakeNumber(ctx, value.Float())
	case reflect.Bool:
		jsv = JSValueMakeBoolean(ctx, value.Bool())
	case reflect.String:
		str := JSStringCreateWithUTF8CString(value.String())
		jsv = JSValueMakeString(ctx, str)
		JSStringRelease(str)
	case reflect.Ptr:
		return toJSValue(ctx, reflect.Indirect(value))
	case reflect.Struct:
		if json, err := json.Marshal(value.Interface()); err == nil {
			str := JSStringCreateWithUTF8CString(string(json))
			jsv = JSValueMakeFromJSONString(ctx, str)
			JSStringRelease(str)
		}
	case reflect.Slice, reflect.Array:
		rets := make([]JSValueRef, value.Len())

		for i := 0; i < value.Len(); i++ {
			rets[i] = toJSValue(ctx, value.Index(i))
		}
		arr := JSObjectMakeArray(ctx, uint(len(rets)), rets, nil)
		jsv = *(*JSValueRef)(unsafe.Pointer(&arr))
	default:
		panic("Not implemented!")
	}

	if err != nil {
		return JSValueMakeNull(ctx)
	}

	return jsv
}

func (w *Window) addFunction(name string) {
	ctx := UlViewGetJSContext(w.view)
	gobj := JSContextGetGlobalObject(ctx)

	fn := JSStringCreateWithUTF8CString(name)
	defer JSStringRelease(fn)

	fname := JSStringCreateWithUTF8CString("name")
	defer JSStringRelease(fname)

	fob := JSObjectMakeFunctionWithCallback(ctx, fn, w.ipcCallback)
	JSObjectSetProperty(ctx, fob, fname, JSValueMakeString(ctx, fname), KJSPropertyAttributeNone, []JSValueRef{})

	val := *(*JSValueRef)(unsafe.Pointer(&fob))

	JSObjectSetProperty(ctx, gobj, fn, val, KJSPropertyAttributeNone, []JSValueRef{})
}

func resizeCallback(ov ULOverlay) func(userData unsafe.Pointer, width uint32, height uint32) {
	return func(userData unsafe.Pointer, width uint32, height uint32) {
		if height > 0 {
			UlOverlayResize(ov, width, height)
		}
	}
}

func serveHandler(handler http.Handler) (string, error) {
	ln, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		return "", err
	}

	go func() {
		if err := http.Serve(ln, handler); err != nil {
			panic(err)
		}
	}()

	return "http://" + ln.Addr().String(), nil
}

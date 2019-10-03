package muon

import (
	"encoding/json"
	"fmt"
	"net"
	"net/http"
	"reflect"
	"unsafe"

	. "github.com/ImVexed/muon/ultralight"
)

type Window struct {
	wnd     ULWindow
	view    ULView
	app     ULApp
	handler http.Handler
	cfg     *Config
}

type ipf struct {
	Function    reflect.Value
	ParamTypes  []reflect.Type
	ReturnTypes []reflect.Type
}

type Config struct {
	Title  string
	Height uint32
	Width  uint32
	Hint   uint32
	X      int32
	Y      int32
}

func New(cfg *Config, handler http.Handler) *Window {
	w := &Window{
		cfg:     cfg,
		handler: handler,
	}

	ufg := UlCreateConfig()
	w.app = UlCreateApp(ufg)
	mm := UlAppGetMainMonitor(w.app)
	w.wnd = UlCreateWindow(mm, w.cfg.Height, w.cfg.Width, false, w.cfg.Hint)

	UlWindowSetTitle(w.wnd, w.cfg.Title)
	UlAppSetWindow(w.app, w.wnd)

	ov := UlCreateOverlay(w.wnd, w.cfg.Height, w.cfg.Width, w.cfg.X, w.cfg.Y)

	UlWindowSetResizeCallback(w.wnd, resizeCallback, unsafe.Pointer(&ov))

	w.view = UlOverlayGetView(ov)

	return w
}

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

	f.ReturnTypes = make([]reflect.Type, t.NumOut())

	for i := 0; i < t.NumOut(); i++ {
		f.ReturnTypes[i] = t.Out(i)
	}

	addFunctionToView(w.view, name, w.makeIPCCallback(f))
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

func (w *Window) makeIPCCallback(f *ipf) func(JSContextRef, JSObjectRef, JSObjectRef, uint, []JSValueRef, []JSValueRef) JSValueRef {
	return func(
		ctx JSContextRef,
		function JSObjectRef,
		thisObject JSObjectRef,
		argumentCount uint,
		arguments []JSValueRef,
		exception []JSValueRef) JSValueRef {

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

		return toJSValue(ctx, val[0])

	}
}

func fromJSValue(ctx JSContextRef, value JSValueRef, rtype reflect.Type) (reflect.Value, error) {
	var rv reflect.Value
	var err error

	if JSValueIsArray(ctx, value) {
		l := JSStringCreateWithUTF8CString("length")
		defer JSStringRelease(l)

		obj := *(*JSObjectRef)(unsafe.Pointer(&value))

		prop := JSObjectGetProperty(ctx, obj, l, nil)
		length := int(JSValueToNumber(ctx, prop, nil))
		values := reflect.MakeSlice(rtype, length, length)

		for i := 0; i < length; i++ {
			ref := JSObjectGetPropertyAtIndex(ctx, obj, uint32(i), nil)

			val, err := fromJSValue(ctx, ref, rtype.Elem())

			if err != nil {
				return reflect.Value{}, err
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
	default:
		panic("Not implemented")
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
		fmt.Println(err.Error())
		return JSValueMakeNull(ctx)
	}
	return jsv
}

func addFunctionToView(view ULView, name string, callback JSObjectCallAsFunctionCallback) {
	ctx := UlViewGetJSContext(view)
	gobj := JSContextGetGlobalObject(ctx)

	fn := JSStringCreateWithUTF8CString(name)
	defer JSStringRelease(fn)

	fob := JSObjectMakeFunctionWithCallback(ctx, fn, callback)

	val := *(*JSValueRef)(unsafe.Pointer(&fob))

	JSObjectSetProperty(ctx, gobj, fn, val, KJSPropertyAttributeNone, []JSValueRef{})
}

func resizeCallback(userData unsafe.Pointer, width uint32, height uint32) {
	overlay := *(*ULOverlay)(userData)
	UlOverlayResize(overlay, width, height)
}

func serveHandler(handler http.Handler) (string, error) {
	ln, err := net.Listen("tcp", "127.0.0.1:0")
	if err != nil {
		return "", err
	}

	go func() {
		panic(http.Serve(ln, handler))
	}()

	return "http://" + ln.Addr().String(), nil
}

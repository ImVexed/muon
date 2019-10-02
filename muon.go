package muon

import (
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
		f.ParamTypes[i] = t.Out(i)
	}

	addFunctionToView(w.view, name, w.makeIPCCallback(f))
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
			switch JSValueGetType(ctx, arguments[i]) {
			case KJSTypeBoolean:
				params[i] = reflect.ValueOf(JSValueToBoolean(ctx, arguments[i]))
			case KJSTypeNumber:
				params[i] = reflect.ValueOf(JSValueToNumber(ctx, arguments[i], exception))
			case KJSTypeString:
				ref := JSValueToStringCopy(ctx, arguments[i], exception)
				params[i] = reflect.ValueOf(*(*string)(unsafe.Pointer(&ref)))
				JSStringRelease(ref)
			default:
				panic("Not implemented!") // TODO: Object JSON
			}
		}

		val := f.Function.Call(params)

		if len(val) > 1 {
			panic("Not implemented!") // TODO: more than 1 return type
		}

		switch val[0].Kind() {
		case reflect.Float64:
			return JSValueMakeNumber(ctx, val[0].Float())
		case reflect.Bool:
			return JSValueMakeBoolean(ctx, val[0].Bool())
		case reflect.String:
			return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(val[0].String()))
		default:
			panic("Not implemented!") // TODO: Object JSON
		}
	}
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

func resizeCallback(user_data unsafe.Pointer, width uint32, height uint32) {
	overlay := *(*ULOverlay)(user_data)
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

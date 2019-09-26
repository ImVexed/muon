//go:generate fileb0x b0x.yml
package main

import (
	"fmt"
	"runtime"
	. "test/ultralight"
	"unsafe"

	"test/fileserver"
)

func jsHandle(
	ctx JSContextRef,
	function JSObjectRef,
	thisObject JSObjectRef,
	argumentCount uint,
	arguments []JSValueRef,
	exception []JSValueRef) JSValueRef {

	fmt.Println("Hello, World!")

	return JSValueMakeString(ctx, JSStringCreateWithUTF8CString(runtime.GOOS))
}

func main() {
	cfg := UlCreateConfig()
	app := UlCreateApp(cfg)
	mm := UlAppGetMainMonitor(app)
	wnd := UlCreateWindow(mm, 500, 500, false, 2|4)

	UlWindowSetTitle(wnd, "Hello, World!")
	UlAppSetWindow(app, wnd)

	ov := UlCreateOverlay(wnd, 500, 500, 0, 0)

	UlWindowSetResizeCallback(wnd, resizeCallback, unsafe.Pointer(&ov))

	view := UlOverlayGetView(ov)

	addr, err := fileserver.Serve()

	if err != nil {
		panic(err)
	}

	fmt.Println(addr)
	url := UlCreateString(addr)
	defer UlDestroyString(url)

	addFunctionToView(view, "getOS", jsHandle)

	UlViewLoadURL(view, url)

	UlAppRun(app)

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

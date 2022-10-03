///
/// @file CAPI.h
///
/// @brief The C-language API for Ultralight
///
/// @author
///
/// This file is a part of Ultralight, a fast, lightweight, HTML UI engine
///
/// Website: <http://ultralig.ht>
///
/// Copyright (C) 2020 Ultralight, Inc. All rights reserved.
///
#ifndef ULTRALIGHT_CAPI_H
#define ULTRALIGHT_CAPI_H

#ifndef __cplusplus
//#include <stdbool.h>
#endif

#include <stddef.h>
#include <JavaScriptCore/JavaScript.h>
#ifdef __OBJC__
#import <AppKit/NSEvent.h>
#endif

#if defined(__WIN32__) || defined(_WIN32)
#  if defined(ULTRALIGHT_IMPLEMENTATION)
#    define ULExport __declspec(dllexport)
#  else
#    define ULExport __declspec(dllimport)
#  endif
#define _thread_local __declspec(thread)
#ifndef _NATIVE_WCHAR_T_DEFINED
#define DISABLE_NATIVE_WCHAR_T
typedef unsigned short ULChar16;
#else
typedef wchar_t ULChar16;
#endif
#else
#  define ULExport __attribute__((visibility("default")))
#define _thread_local __thread
typedef unsigned short ULChar16;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct C_Config* ULConfig;
typedef struct C_Renderer* ULRenderer;
typedef struct C_Session* ULSession;
typedef struct C_View* ULView;
typedef struct C_Bitmap* ULBitmap;
typedef struct C_String* ULString;
typedef struct C_Buffer* ULBuffer;
typedef struct C_KeyEvent* ULKeyEvent;
typedef struct C_MouseEvent* ULMouseEvent;
typedef struct C_ScrollEvent* ULScrollEvent;
typedef struct C_Surface* ULSurface;
typedef struct C_Surface* ULBitmapSurface;

typedef enum {
  kMessageSource_XML = 0,
  kMessageSource_JS,
  kMessageSource_Network,
  kMessageSource_ConsoleAPI,
  kMessageSource_Storage,
  kMessageSource_AppCache,
  kMessageSource_Rendering,
  kMessageSource_CSS,
  kMessageSource_Security,
  kMessageSource_ContentBlocker,
  kMessageSource_Other,
} ULMessageSource;

typedef enum {
  kMessageLevel_Log = 1,
  kMessageLevel_Warning = 2,
  kMessageLevel_Error = 3,
  kMessageLevel_Debug = 4,
  kMessageLevel_Info = 5,
} ULMessageLevel;

typedef enum {
  kCursor_Pointer = 0,
  kCursor_Cross,
  kCursor_Hand,
  kCursor_IBeam,
  kCursor_Wait,
  kCursor_Help,
  kCursor_EastResize,
  kCursor_NorthResize,
  kCursor_NorthEastResize,
  kCursor_NorthWestResize,
  kCursor_SouthResize,
  kCursor_SouthEastResize,
  kCursor_SouthWestResize,
  kCursor_WestResize,
  kCursor_NorthSouthResize,
  kCursor_EastWestResize,
  kCursor_NorthEastSouthWestResize,
  kCursor_NorthWestSouthEastResize,
  kCursor_ColumnResize,
  kCursor_RowResize,
  kCursor_MiddlePanning,
  kCursor_EastPanning,
  kCursor_NorthPanning,
  kCursor_NorthEastPanning,
  kCursor_NorthWestPanning,
  kCursor_SouthPanning,
  kCursor_SouthEastPanning,
  kCursor_SouthWestPanning,
  kCursor_WestPanning,
  kCursor_Move,
  kCursor_VerticalText,
  kCursor_Cell,
  kCursor_ContextMenu,
  kCursor_Alias,
  kCursor_Progress,
  kCursor_NoDrop,
  kCursor_Copy,
  kCursor_None,
  kCursor_NotAllowed,
  kCursor_ZoomIn,
  kCursor_ZoomOut,
  kCursor_Grab,
  kCursor_Grabbing,
  kCursor_Custom
} ULCursor;

typedef enum {
  ///
  /// Alpha channel only, 8-bits per pixel.
  ///
  /// Encoding: 8-bits per channel, unsigned normalized.
  ///
  /// Color-space: Linear (no gamma), alpha-coverage only.
  ///
  kBitmapFormat_A8_UNORM,

  ///
  /// Blue Green Red Alpha channels, 32-bits per pixel.
  /// 
  /// Encoding: 8-bits per channel, unsigned normalized.
  ///
  /// Color-space: sRGB gamma with premultiplied linear alpha channel.
  ///
  kBitmapFormat_BGRA8_UNORM_SRGB
} ULBitmapFormat;

typedef enum {
  ///
  /// Key-Down event type. (Does not trigger accelerator commands in WebCore)
  ///
  /// @NOTE: You should probably use RawKeyDown instead when a physical key
  ///        is pressed. This member is only here for historic compatibility
  ///        with WebCore's key event types.
  ///
  kKeyEventType_KeyDown,

  ///
  /// Key-Up event type. Use this when a physical key is released.
  ///
  kKeyEventType_KeyUp,

  ///
  /// Raw Key-Down type. Use this when a physical key is pressed.
  ///
  /// @NOTE: You should use RawKeyDown for physical key presses since it
  ///        allows WebCore to do additional command translation.
  ///
  kKeyEventType_RawKeyDown,

  ///
  /// Character input event type. Use this when the OS generates text from
  /// a physical key being pressed (eg, WM_CHAR on Windows).
  ///
  kKeyEventType_Char,
} ULKeyEventType;

typedef enum {
  kMouseEventType_MouseMoved,
  kMouseEventType_MouseDown,
  kMouseEventType_MouseUp,
} ULMouseEventType;

typedef enum {
  kMouseButton_None = 0,
  kMouseButton_Left,
  kMouseButton_Middle,
  kMouseButton_Right,
} ULMouseButton;

typedef enum {
  kScrollEventType_ScrollByPixel,
  kScrollEventType_ScrollByPage,
} ULScrollEventType;

typedef enum {
  kFaceWinding_Clockwise,
  kFaceWindow_CounterClockwise,
} ULFaceWinding;

typedef enum {
  ///
  /// Lighter hinting algorithm-- glyphs are slightly fuzzier but better
  /// resemble their original shape. This is achieved by snapping glyphs to the
  /// pixel grid only vertically which better preserves inter-glyph spacing.
  ///
  kFontHinting_Smooth,

  ///
  /// Default hinting algorithm-- offers a good balance between sharpness and
  /// shape at smaller font sizes.
  ///
  kFontHinting_Normal,

  ///
  /// Strongest hinting algorithm-- outputs only black/white glyphs. The result
  /// is usually unpleasant if the underlying TTF does not contain hints for
  /// this type of rendering.
  ///
  kFontHinting_Monochrome,
} ULFontHinting;

typedef struct {
  float left;
  float top;
  float right;
  float bottom;
} ULRect;

typedef struct {
  int left;
  int top;
  int right;
  int bottom;
} ULIntRect;

typedef struct {
  bool is_empty;
  unsigned int width;
  unsigned int height;
  unsigned int texture_id;
  unsigned int texture_width;
  unsigned int texture_height;
  ULBitmapFormat texture_format;
  ULRect uv_coords;
  unsigned int render_buffer_id;
} ULRenderTarget;


/******************************************************************************
 * API Note:
 *
 * You should only destroy objects that you explicitly create. Do not destroy
 * any objects returned from the API or callbacks unless otherwise noted.
 *****************************************************************************/

/******************************************************************************
 * Version
 *****************************************************************************/

///
/// Get the version string of the library in MAJOR.MINOR.PATCH format.
///
ULExport const char* ulVersionString();

///
/// Get the numeric major version of the library.
///
ULExport unsigned int ulVersionMajor();

///
/// Get the numeric minor version of the library.
///
ULExport unsigned int ulVersionMinor();

///
/// Get the numeric patch version of the library.
///
ULExport unsigned int ulVersionPatch();

/******************************************************************************
 * Config
 *****************************************************************************/

///
/// Create config with default values (see <Ultralight/platform/Config.h>).
///
ULExport ULConfig ulCreateConfig();

///
/// Destroy config.
///
ULExport void ulDestroyConfig(ULConfig config);

///
/// Set the file path to the directory that contains Ultralight's bundled
/// resources (eg, cacert.pem and other localized resources). 
///
ULExport void ulConfigSetResourcePath(ULConfig config, ULString resource_path);

///
/// Set the file path to a writable directory that will be used to store
/// cookies, cached resources, and other persistent data.
///
ULExport void ulConfigSetCachePath(ULConfig config, ULString cache_path);

///
/// When enabled, each View will be rendered to an offscreen GPU texture
/// using the GPU driver set in ulPlatformSetGPUDriver. You can fetch
/// details for the texture via ulViewGetRenderTarget.
///
/// When disabled (the default), each View will be rendered to an offscreen
/// pixel buffer. This pixel buffer can optionally be provided by the user--
/// for more info see ulViewGetSurface.
///
ULExport void ulConfigSetUseGPURenderer(ULConfig config, bool use_gpu);

///
/// Set the amount that the application DPI has been scaled, used for
/// scaling device coordinates to pixels and oversampling raster shapes
/// (Default = 1.0).
///
ULExport void ulConfigSetDeviceScale(ULConfig config, double value);

///
/// The winding order for front-facing triangles. @see FaceWinding
///
/// Note: This is only used with custom GPUDrivers
///
ULExport void ulConfigSetFaceWinding(ULConfig config, ULFaceWinding winding);

///
/// Set whether images should be enabled (Default = True).
///
ULExport void ulConfigSetEnableImages(ULConfig config, bool enabled);

///
/// Set whether JavaScript should be eanbled (Default = True).
///
ULExport void ulConfigSetEnableJavaScript(ULConfig config, bool enabled);

///
/// The hinting algorithm to use when rendering fonts. (Default = kFontHinting_Normal)
/// @see ULFontHinting
///
ULExport void ulConfigSetFontHinting(ULConfig config, ULFontHinting font_hinting);

///
/// The gamma to use when compositing font glyphs, change this value to
/// adjust contrast (Adobe and Apple prefer 1.8, others may prefer 2.2).
/// (Default = 1.8)
///
ULExport void ulConfigSetFontGamma(ULConfig config, double font_gamma);

///
/// Set default font-family to use (Default = Times New Roman).
///
ULExport void ulConfigSetFontFamilyStandard(ULConfig config,
                                            ULString font_name);

///
/// Set default font-family to use for fixed fonts, eg <pre> and <code>
/// (Default = Courier New).
///
ULExport void ulConfigSetFontFamilyFixed(ULConfig config, ULString font_name);

///
/// Set default font-family to use for serif fonts (Default = Times New Roman).
///
ULExport void ulConfigSetFontFamilySerif(ULConfig config, ULString font_name);

///
/// Set default font-family to use for sans-serif fonts (Default = Arial).
///
ULExport void ulConfigSetFontFamilySansSerif(ULConfig config,
                                             ULString font_name);

///
/// Set user agent string (See <Ultralight/platform/Config.h> for the default).
///
ULExport void ulConfigSetUserAgent(ULConfig config, ULString agent_string);

///
/// Set user stylesheet (CSS) (Default = Empty).
///
ULExport void ulConfigSetUserStylesheet(ULConfig config, ULString css_string);

///
/// Set whether or not we should continuously repaint any Views or compositor
/// layers, regardless if they are dirty or not. This is mainly used to
/// diagnose painting/shader issues. (Default = False)
///
ULExport void ulConfigSetForceRepaint(ULConfig config, bool enabled);

///
/// Set the amount of time to wait before triggering another repaint when a
/// CSS animation is active. (Default = 1.0 / 60.0)
///
ULExport void ulConfigSetAnimationTimerDelay(ULConfig config, double delay);

///
/// When a smooth scroll animation is active, the amount of time (in seconds)
/// to wait before triggering another repaint. Default is 60 Hz.
///
ULExport void ulConfigSetScrollTimerDelay(ULConfig config, double delay);

///
/// The amount of time (in seconds) to wait before running the recycler (will
/// attempt to return excess memory back to the system). (Default = 4.0)
///
ULExport void ulConfigSetRecycleDelay(ULConfig config, double delay);

///
/// Set the size of WebCore's memory cache for decoded images, scripts, and
/// other assets in bytes. (Default = 64 * 1024 * 1024)
///
ULExport void ulConfigSetMemoryCacheSize(ULConfig config, unsigned int size);

///
/// Set the number of pages to keep in the cache. (Default = 0)
///
ULExport void ulConfigSetPageCacheSize(ULConfig config, unsigned int size);

///
/// JavaScriptCore tries to detect the system's physical RAM size to set
/// reasonable allocation limits. Set this to anything other than 0 to
/// override the detected value. Size is in bytes.
///
/// This can be used to force JavaScriptCore to be more conservative with
/// its allocation strategy (at the cost of some performance).
///
ULExport void ulConfigSetOverrideRAMSize(ULConfig config, unsigned int size);

///
/// The minimum size of large VM heaps in JavaScriptCore. Set this to a
/// lower value to make these heaps start with a smaller initial value.
///
ULExport void ulConfigSetMinLargeHeapSize(ULConfig config, unsigned int size);

///
/// The minimum size of small VM heaps in JavaScriptCore. Set this to a
/// lower value to make these heaps start with a smaller initial value.
///
ULExport void ulConfigSetMinSmallHeapSize(ULConfig config, unsigned int size);

/******************************************************************************
 * Renderer
 *****************************************************************************/

///
/// Create the Ultralight Renderer directly.
///
/// Unlike ulCreateApp(), this does not use any native windows for drawing
/// and allows you to manage your own runloop and painting. This method is
/// recommended for those wishing to integrate the library into a game.
///
/// This singleton manages the lifetime of all Views and coordinates all
/// painting, rendering, network requests, and event dispatch.
///
/// You should only call this once per process lifetime.
///
/// You shoud set up your platform handlers (eg, ulPlatformSetLogger,
/// ulPlatformSetFileSystem, etc.) before calling this.
///
/// You will also need to define a font loader before calling this-- 
/// as of this writing (v1.2) the only way to do this in C API is by calling
/// ulEnablePlatformFontLoader() (available in <AppCore/CAPI.h>).
///
/// @NOTE:  You should not call this if you are using ulCreateApp(), it
///         creates its own renderer and provides default implementations for
///         various platform handlers automatically.
///
ULExport ULRenderer ulCreateRenderer(ULConfig config);

///
/// Destroy the renderer.
///
ULExport void ulDestroyRenderer(ULRenderer renderer);

///
/// Update timers and dispatch internal callbacks (JavaScript and network).
///
ULExport void ulUpdate(ULRenderer renderer);

///
/// Render all active Views.
///
ULExport void ulRender(ULRenderer renderer);

///
/// Attempt to release as much memory as possible. Don't call this from any
/// callbacks or driver code.
///
ULExport void ulPurgeMemory(ULRenderer renderer);

///
/// Print detailed memory usage statistics to the log.
/// (@see ulPlatformSetLogger)
///
ULExport void ulLogMemoryUsage(ULRenderer renderer);

/******************************************************************************
 * Session
 *****************************************************************************/

///
/// Create a Session to store local data in (such as cookies, local storage,
/// application cache, indexed db, etc).
///
ULExport ULSession ulCreateSession(ULRenderer renderer, bool is_persistent,
                                   ULString name);

///
/// Destroy a Session.
///
ULExport void ulDestroySession(ULSession session);

///
/// Get the default session (persistent session named "default").
///
/// @note  This session is owned by the Renderer, you shouldn't destroy it.
///
ULExport ULSession ulDefaultSession(ULRenderer renderer);

///
/// Whether or not is persistent (backed to disk).
///
ULExport bool ulSessionIsPersistent(ULSession session);

///
/// Unique name identifying the session (used for unique disk path).
///
ULExport ULString ulSessionGetName(ULSession session);

///
/// Unique numeric Id for the session.
///
ULExport unsigned long long ulSessionGetId(ULSession session);

///
/// The disk path to write to (used by persistent sessions only).
///
ULExport ULString ulSessionGetDiskPath(ULSession session);

/******************************************************************************
 * View
 *****************************************************************************/

///
/// Create a View with certain size (in pixels).
///
/// @note  You can pass null to 'session' to use the default session.
///
ULExport ULView ulCreateView(ULRenderer renderer, unsigned int width,
                             unsigned int height, bool transparent,
                             ULSession session, bool force_cpu_renderer);

///
/// Destroy a View.
///
ULExport void ulDestroyView(ULView view);

///
/// Get current URL.
///
/// @note Don't destroy the returned string, it is owned by the View.
///
ULExport ULString ulViewGetURL(ULView view);

///
/// Get current title.
///
/// @note Don't destroy the returned string, it is owned by the View.
///
ULExport ULString ulViewGetTitle(ULView view);

///
/// Get the width, in pixels.
///
ULExport unsigned int ulViewGetWidth(ULView view);

///
/// Get the height, in pixels.
///
ULExport unsigned int ulViewGetHeight(ULView view);

///
/// Check if main frame is loading.
///
ULExport bool ulViewIsLoading(ULView view);

///
/// Get the RenderTarget for the View.
///
/// @note  Only valid when the GPU renderer is enabled in Config.
///
ULExport ULRenderTarget ulViewGetRenderTarget(ULView view);

///
/// Get the Surface for the View (native pixel buffer container).
///
/// @note  Only valid when the GPU renderer is disabled in Config.
///
///        (Will return a nullptr when the GPU renderer is enabled.)
///
///        The default Surface is BitmapSurface but you can provide your
///        own Surface implementation via ulPlatformSetSurfaceDefinition.
///
///        When using the default Surface, you can retrieve the underlying
///        bitmap by casting ULSurface to ULBitmapSurface and calling
///        ulBitmapSurfaceGetBitmap().
///
ULExport ULSurface ulViewGetSurface(ULView view);

///
/// Load a raw string of HTML.
///
ULExport void ulViewLoadHTML(ULView view, ULString html_string);

///
/// Load a URL into main frame.
///
ULExport void ulViewLoadURL(ULView view, ULString url_string);

///
/// Resize view to a certain width and height (in pixels).
///
ULExport void ulViewResize(ULView view, unsigned int width,
                           unsigned int height);

///
/// Acquire the page's JSContext for use with JavaScriptCore API.
/// 
/// @note  This call locks the context for the current thread. You should
///        call ulViewUnlockJSContext() after using the context so other
///        worker threads can modify JavaScript state.
///
/// @note  The lock is recusive, it's okay to call this multiple times as long
///        as you call ulViewUnlockJSContext() the same number of times.
///
ULExport JSContextRef ulViewLockJSContext(ULView view);

///
/// Unlock the page's JSContext after a previous call to ulViewLockJSContext().
///
ULExport void ulViewUnlockJSContext(ULView view);

///
/// Evaluate a string of JavaScript and return result.
///
/// @param  js_string  The string of JavaScript to evaluate.
///
/// @param  exception  The address of a ULString to store a description of the
///                    last exception. Pass NULL to ignore this. Don't destroy
///                    the exception string returned, it's owned by the View.
///
/// @note Don't destroy the returned string, it's owned by the View. This value
///       is reset with every call-- if you want to retain it you should copy
///       the result to a new string via ulCreateStringFromCopy().
///
/// @note An example of using this API:
///       <pre>
///         ULString script = ulCreateString("1 + 1");
///         ULString exception;
///         ULString result = ulViewEvaluateScript(view, script, &exception);
///         /* Use the result ("2") and exception description (if any) here. */
///         ulDestroyString(script);
///       </pre>
///
ULExport ULString ulViewEvaluateScript(ULView view, ULString js_string, ULString* exception);

///
/// Check if can navigate backwards in history.
///
ULExport bool ulViewCanGoBack(ULView view);

///
/// Check if can navigate forwards in history.
///
ULExport bool ulViewCanGoForward(ULView view);

///
/// Navigate backwards in history.
///
ULExport void ulViewGoBack(ULView view);

///
/// Navigate forwards in history.
///
ULExport void ulViewGoForward(ULView view);

///
/// Navigate to arbitrary offset in history.
///
ULExport void ulViewGoToHistoryOffset(ULView view, int offset);

///
/// Reload current page.
///
ULExport void ulViewReload(ULView view);

///
/// Stop all page loads.
///
ULExport void ulViewStop(ULView view);

///
/// Give focus to the View.
///
/// You should call this to give visual indication that the View has input
/// focus (changes active text selection colors, for example).
///
ULExport void ulViewFocus(ULView view);

///
/// Remove focus from the View and unfocus any focused input elements.
///
/// You should call this to give visual indication that the View has lost
/// input focus.
///
ULExport void ulViewUnfocus(ULView view);

///
/// Whether or not the View has focus.
///
ULExport bool ulViewHasFocus(ULView view);

///
/// Whether or not the View has an input element with visible keyboard focus
/// (indicated by a blinking caret).
///
/// You can use this to decide whether or not the View should consume
/// keyboard input events (useful in games with mixed UI and key handling).
///
ULExport bool ulViewHasInputFocus(ULView view);

///
/// Fire a keyboard event.
///
ULExport void ulViewFireKeyEvent(ULView view, ULKeyEvent key_event);

///
/// Fire a mouse event.
///
ULExport void ulViewFireMouseEvent(ULView view, ULMouseEvent mouse_event);

///
/// Fire a scroll event.
///
ULExport void ulViewFireScrollEvent(ULView view, ULScrollEvent scroll_event);

typedef void
(*ULChangeTitleCallback) (void* user_data, ULView caller, ULString title);

///
/// Set callback for when the page title changes.
///
ULExport void ulViewSetChangeTitleCallback(ULView view,
                                           ULChangeTitleCallback callback,
                                           void* user_data);

typedef void
(*ULChangeURLCallback) (void* user_data, ULView caller, ULString url);

///
/// Set callback for when the page URL changes.
///
ULExport void ulViewSetChangeURLCallback(ULView view,
                                         ULChangeURLCallback callback,
                                         void* user_data);

typedef void
(*ULChangeTooltipCallback) (void* user_data, ULView caller, ULString tooltip);

///
/// Set callback for when the tooltip changes (usually result of a mouse hover).
///
ULExport void ulViewSetChangeTooltipCallback(ULView view,
                                             ULChangeTooltipCallback callback,
                                             void* user_data);

typedef void
(*ULChangeCursorCallback) (void* user_data, ULView caller, ULCursor cursor);

///
/// Set callback for when the mouse cursor changes.
///
ULExport void ulViewSetChangeCursorCallback(ULView view,
                                            ULChangeCursorCallback callback,
                                            void* user_data);

typedef void
(*ULAddConsoleMessageCallback) (void* user_data, ULView caller,
                                ULMessageSource source, ULMessageLevel level,
                                ULString message, unsigned int line_number,
                                unsigned int column_number,
                                ULString source_id);

///
/// Set callback for when a message is added to the console (useful for
/// JavaScript / network errors and debugging).
///
ULExport void ulViewSetAddConsoleMessageCallback(ULView view,
                                          ULAddConsoleMessageCallback callback,
                                          void* user_data);

typedef ULView
(*ULCreateChildViewCallback) (void* user_data, ULView caller,
  ULString opener_url, ULString target_url, bool is_popup,
  ULIntRect popup_rect);

///
/// Set callback for when the page wants to create a new View.
///
/// This is usually the result of a user clicking a link with target="_blank"
/// or by JavaScript calling window.open(url).
///
/// To allow creation of these new Views, you should create a new View in
/// this callback, resize it to your container,
/// and return it. You are responsible for displaying the returned View.
///
/// You should return NULL if you want to block the action.
///
ULExport void ulViewSetCreateChildViewCallback(ULView view,
                                            ULCreateChildViewCallback callback,
                                            void* user_data);

typedef void
(*ULBeginLoadingCallback) (void* user_data, ULView caller,
  unsigned long long frame_id, bool is_main_frame, ULString url);

///
/// Set callback for when the page begins loading a new URL into a frame.
///
ULExport void ulViewSetBeginLoadingCallback(ULView view,
                                            ULBeginLoadingCallback callback,
                                            void* user_data);

typedef void
(*ULFinishLoadingCallback) (void* user_data, ULView caller,
  unsigned long long frame_id, bool is_main_frame, ULString url);

///
/// Set callback for when the page finishes loading a URL into a frame.
///
ULExport void ulViewSetFinishLoadingCallback(ULView view,
                                             ULFinishLoadingCallback callback,
                                             void* user_data);

typedef void
(*ULFailLoadingCallback) (void* user_data, ULView caller,
  unsigned long long frame_id, bool is_main_frame, ULString url,
  ULString description, ULString error_domain, int error_code);

///
/// Set callback for when an error occurs while loading a URL into a frame.
///
ULExport void ulViewSetFailLoadingCallback(ULView view,
                                           ULFailLoadingCallback callback,
                                           void* user_data);

typedef void
(*ULWindowObjectReadyCallback) (void* user_data, ULView caller,
  unsigned long long frame_id, bool is_main_frame, ULString url);

///
/// Set callback for when the JavaScript window object is reset for a new
/// page load.
///
/// This is called before any scripts are executed on the page and is the
/// earliest time to setup any initial JavaScript state or bindings.
///
/// The document is not guaranteed to be loaded/parsed at this point. If
/// you need to make any JavaScript calls that are dependent on DOM elements
/// or scripts on the page, use DOMReady instead.
///
/// The window object is lazily initialized (this will not be called on pages
/// with no scripts).
///
ULExport void ulViewSetWindowObjectReadyCallback(ULView view,
                                          ULWindowObjectReadyCallback callback,
                                          void* user_data);

typedef void
(*ULDOMReadyCallback) (void* user_data, ULView caller,
  unsigned long long frame_id, bool is_main_frame, ULString url);

///
/// Set callback for when all JavaScript has been parsed and the document is
/// ready.
///
/// This is the best time to make any JavaScript calls that are dependent on
/// DOM elements or scripts on the page.
///
ULExport void ulViewSetDOMReadyCallback(ULView view,
                                        ULDOMReadyCallback callback,
                                        void* user_data);

typedef void
(*ULUpdateHistoryCallback) (void* user_data, ULView caller);

///
/// Set callback for when the history (back/forward state) is modified.
///
ULExport void ulViewSetUpdateHistoryCallback(ULView view,
                                             ULUpdateHistoryCallback callback,
                                             void* user_data);

///
/// Set whether or not a view should be repainted during the next call to
/// ulRender.
///
/// @note  This flag is automatically set whenever the page content changes
///        but you can set it directly in case you need to force a repaint.
///
ULExport void ulViewSetNeedsPaint(ULView view, bool needs_paint);

///
/// Whether or not a view should be painted during the next call to ulRender.
///
ULExport bool ulViewGetNeedsPaint(ULView view);

///
/// Create an inspector for this View, this is useful for debugging and
/// inspecting pages locally. This will only succeed if you have the
/// inspector assets in your filesystem-- the inspector will look for
/// file:///inspector/Main.html when it loads.
///
/// @note  The initial dimensions of the returned View are 10x10, you should
///        call ulViewResize on the returned View to resize it to your desired
///        dimensions.
///
/// @note  You will need to call ulDestroyView on the returned instance
///        when you're done using it.
///
ULExport ULView ulViewCreateInspectorView(ULView view);

/******************************************************************************
 * String
 *****************************************************************************/

///
/// Create string from null-terminated ASCII C-string.
///
ULExport ULString ulCreateString(const char* str);

///
/// Create string from UTF-8 buffer.
///
ULExport ULString ulCreateStringUTF8(const char* str, size_t len);

///
/// Create string from UTF-16 buffer.
///
ULExport ULString ulCreateStringUTF16(ULChar16* str, size_t len);

///
/// Create string from copy of existing string.
///
ULExport ULString ulCreateStringFromCopy(ULString str);

///
/// Destroy string (you should destroy any strings you explicitly Create).
///
ULExport void ulDestroyString(ULString str);

///
/// Get internal UTF-16 buffer data.
///
ULExport ULChar16* ulStringGetData(ULString str);

///
/// Get length in UTF-16 characters.
///
ULExport size_t ulStringGetLength(ULString str);

///
/// Whether this string is empty or not.
///
ULExport bool ulStringIsEmpty(ULString str);

///
/// Replaces the contents of 'str' with the contents of 'new_str'
///
ULExport void ulStringAssignString(ULString str, ULString new_str);

///
/// Replaces the contents of 'str' with the contents of a C-string.
///
ULExport void ulStringAssignCString(ULString str, const char* c_str);

/******************************************************************************
 * Bitmap
 *****************************************************************************/

///
/// Create empty bitmap.
///
ULExport ULBitmap ulCreateEmptyBitmap();

///
/// Create bitmap with certain dimensions and pixel format.
///
ULExport ULBitmap ulCreateBitmap(unsigned int width, unsigned int height,
                                 ULBitmapFormat format);

///
/// Create bitmap from existing pixel buffer. @see Bitmap for help using
/// this function.
///
ULExport ULBitmap ulCreateBitmapFromPixels(unsigned int width,
                                           unsigned int height,
                                           ULBitmapFormat format, 
                                           unsigned int row_bytes,
                                           const void* pixels, size_t size,
                                           bool should_copy);

///
/// Create bitmap from copy.
///
ULExport ULBitmap ulCreateBitmapFromCopy(ULBitmap existing_bitmap);

///
/// Destroy a bitmap (you should only destroy Bitmaps you have explicitly
/// created via one of the creation functions above.
///
ULExport void ulDestroyBitmap(ULBitmap bitmap);

///
/// Get the width in pixels.
///
ULExport unsigned int ulBitmapGetWidth(ULBitmap bitmap);

///
/// Get the height in pixels.
///
ULExport unsigned int ulBitmapGetHeight(ULBitmap bitmap);

///
/// Get the pixel format.
///
ULExport ULBitmapFormat ulBitmapGetFormat(ULBitmap bitmap);

///
/// Get the bytes per pixel.
///
ULExport unsigned int ulBitmapGetBpp(ULBitmap bitmap);

///
/// Get the number of bytes per row.
///
ULExport unsigned int ulBitmapGetRowBytes(ULBitmap bitmap);

///
/// Get the size in bytes of the underlying pixel buffer.
///
ULExport size_t ulBitmapGetSize(ULBitmap bitmap);

///
/// Whether or not this bitmap owns its own pixel buffer.
///
ULExport bool ulBitmapOwnsPixels(ULBitmap bitmap);

///
/// Lock pixels for reading/writing, returns pointer to pixel buffer.
///
ULExport void* ulBitmapLockPixels(ULBitmap bitmap);

///
/// Unlock pixels after locking.
///
ULExport void ulBitmapUnlockPixels(ULBitmap bitmap);

///
/// Get raw pixel buffer-- you should only call this if Bitmap is already
/// locked.
///
ULExport void* ulBitmapRawPixels(ULBitmap bitmap);

///
/// Whether or not this bitmap is empty.
///
ULExport bool ulBitmapIsEmpty(ULBitmap bitmap);

///
/// Reset bitmap pixels to 0.
///
ULExport void ulBitmapErase(ULBitmap bitmap);

///
/// Write bitmap to a PNG on disk.
///
ULExport bool ulBitmapWritePNG(ULBitmap bitmap, const char* path);

///
/// This converts a BGRA bitmap to RGBA bitmap and vice-versa by swapping
/// the red and blue channels.
///
ULExport void ulBitmapSwapRedBlueChannels(ULBitmap bitmap);

/******************************************************************************
* Key Event
******************************************************************************/

///
/// Create a key event, @see KeyEvent for help with the following parameters.
///
ULExport ULKeyEvent ulCreateKeyEvent(ULKeyEventType type,
                                     unsigned int modifiers,
                                     int virtual_key_code, int native_key_code,
                                     ULString text, ULString unmodified_text,
                                     bool is_keypad, bool is_auto_repeat,
                                     bool is_system_key);

#ifdef _WIN32
///
/// Create a key event from native Windows event.
///
ULExport ULKeyEvent ulCreateKeyEventWindows(ULKeyEventType type,
                                            uintptr_t wparam, intptr_t lparam,
                                            bool is_system_key);
#endif

#ifdef __OBJC__
///
/// Create a key event from native macOS event.
///
ULExport ULKeyEvent ulCreateKeyEventMacOS(NSEvent* evt);
#endif

///
/// Destroy a key event.
///
ULExport void ulDestroyKeyEvent(ULKeyEvent evt);

/******************************************************************************
 * Mouse Event
 *****************************************************************************/

///
/// Create a mouse event, @see MouseEvent for help using this function.
///
ULExport ULMouseEvent ulCreateMouseEvent(ULMouseEventType type, int x, int y,
                                         ULMouseButton button);

///
/// Destroy a mouse event.
///
ULExport void ulDestroyMouseEvent(ULMouseEvent evt);

/******************************************************************************
 * Scroll Event
 *****************************************************************************/

///
/// Create a scroll event, @see ScrollEvent for help using this function.
///
ULExport ULScrollEvent ulCreateScrollEvent(ULScrollEventType type, int delta_x,
                                           int delta_y);

///
/// Destroy a scroll event.
///
ULExport void ulDestroyScrollEvent(ULScrollEvent evt);

/******************************************************************************
 * Rect
 *****************************************************************************/

///
/// Whether or not a ULRect is empty (all members equal to 0)
///
ULExport bool ulRectIsEmpty(ULRect rect);

///
/// Create an empty ULRect (all members equal to 0)
///
ULExport ULRect ulRectMakeEmpty();

/******************************************************************************
 * IntRect
 *****************************************************************************/

 ///
 /// Whether or not a ULIntRect is empty (all members equal to 0)
 ///
ULExport bool ulIntRectIsEmpty(ULIntRect rect);

///
/// Create an empty ULIntRect (all members equal to 0)
///
ULExport ULIntRect ulIntRectMakeEmpty();

/******************************************************************************
 * Surface
 *****************************************************************************/

///
/// Width (in pixels).
///
ULExport unsigned int ulSurfaceGetWidth(ULSurface surface);

///
/// Height (in pixels).
///
ULExport unsigned int ulSurfaceGetHeight(ULSurface surface);

///
/// Number of bytes between rows (usually width * 4)
///
ULExport unsigned int ulSurfaceGetRowBytes(ULSurface surface);

///
/// Size in bytes.
///
ULExport size_t ulSurfaceGetSize(ULSurface surface);

///
/// Lock the pixel buffer and get a pointer to the beginning of the data
/// for reading/writing.
///
/// Native pixel format is premultiplied BGRA 32-bit (8 bits per channel).
///
ULExport void* ulSurfaceLockPixels(ULSurface surface);

///
/// Unlock the pixel buffer.
///
ULExport void ulSurfaceUnlockPixels(ULSurface surface);

///
/// Resize the pixel buffer to a certain width and height (both in pixels).
///
/// This should never be called while pixels are locked. 
///
ULExport void ulSurfaceResize(ULSurface surface, unsigned int width, unsigned int height);

///
/// Set the dirty bounds to a certain value.
///
/// This is called after the Renderer paints to an area of the pixel buffer.
/// (The new value will be joined with the existing dirty_bounds())
///
ULExport void ulSurfaceSetDirtyBounds(ULSurface surface, ULIntRect bounds);

///
/// Get the dirty bounds.
///
/// This value can be used to determine which portion of the pixel buffer has
/// been updated since the last call to ulSurfaceClearDirtyBounds().
///
/// The general algorithm to determine if a Surface needs display is:
/// <pre>
///   if (!ulIntRectIsEmpty(ulSurfaceGetDirtyBounds(surface))) {
///       // Surface pixels are dirty and needs display.
///       // Cast Surface to native Surface and use it here (pseudo code)
///       DisplaySurface(surface);
///
///       // Once you're done, clear the dirty bounds:
///       ulSurfaceClearDirtyBounds(surface);
///  }
///  </pre>
///
ULExport ULIntRect ulSurfaceGetDirtyBounds(ULSurface surface);

///
/// Clear the dirty bounds.
///
/// You should call this after you're done displaying the Surface.
///
ULExport void ulSurfaceClearDirtyBounds(ULSurface surface);

///
/// Get the underlying user data pointer (this is only valid if you have
/// set a custom surface implementation via ulPlatformSetSurfaceDefinition).
///
/// This will return nullptr if this surface is the default ULBitmapSurface.
///
ULExport void* ulSurfaceGetUserData(ULSurface surface);

/******************************************************************************
 * BitmapSurface
 *****************************************************************************/

///
/// Get the underlying Bitmap from the default Surface.
///
/// @note  Do not call ulDestroyBitmap() on the returned value, it is owned
///        by the surface.
///
ULExport ULBitmap ulBitmapSurfaceGetBitmap(ULBitmapSurface surface);

/******************************************************************************
 * Surface Definition
 *****************************************************************************/

///
/// The callback invoked when a Surface is created.
///
/// @param  width  The width in pixels.
/// @param  height  The height in pixels.
///
/// @return  This callback should return a pointer to user-defined data for the
///          instance. This user data pointer will be passed to all other
///          callbacks when operating on the instance.
///
typedef void*
(*ULSurfaceDefinitionCreateCallback) (unsigned int width, unsigned int height);

///
/// The callback invoked when a Surface is destroyed.
///
/// @param  user_data  User data pointer uniquely identifying the surface.
///
typedef void
(*ULSurfaceDefinitionDestroyCallback) (void* user_data);

///
/// The callback invoked when a Surface's width (in pixels) is requested.
///
/// @param  user_data  User data pointer uniquely identifying the surface.
///
typedef unsigned int
(*ULSurfaceDefinitionGetWidthCallback) (void* user_data);

///
/// The callback invoked when a Surface's height (in pixels) is requested.
///
/// @param  user_data  User data pointer uniquely identifying the surface.
///
typedef unsigned int
(*ULSurfaceDefinitionGetHeightCallback) (void* user_data);

///
/// The callback invoked when a Surface's row bytes is requested.
///
/// @note  This value is also known as "stride". Usually width * 4.
///
/// @param  user_data  User data pointer uniquely identifying the surface.
///
typedef unsigned int
(*ULSurfaceDefinitionGetRowBytesCallback) (void* user_data);

///
/// The callback invoked when a Surface's size (in bytes) is requested.
///
/// @param  user_data  User data pointer uniquely identifying the surface.
///
typedef size_t
(*ULSurfaceDefinitionGetSizeCallback) (void* user_data);

///
/// The callback invoked when a Surface's pixel buffer is requested to be
/// locked for reading/writing (should return a pointer to locked bytes).
///
/// @param  user_data  User data pointer uniquely identifying the surface.
///
typedef void*
(*ULSurfaceDefinitionLockPixelsCallback) (void* user_data);

///
/// The callback invoked when a Surface's pixel buffer is requested to be
/// unlocked after previously being locked.
///
/// @param  user_data  User data pointer uniquely identifying the surface.
///
typedef void
(*ULSurfaceDefinitionUnlockPixelsCallback) (void* user_data);

///
/// The callback invoked when a Surface is requested to be resized to a
/// certain width/height.
///
/// @param  user_data  User data pointer uniquely identifying the surface.
/// @param  width      Width in pixels.
/// @param  height     Height in pixels.
///
typedef void
(*ULSurfaceDefinitionResizeCallback) (void* user_data, unsigned int width, unsigned int height);

typedef struct {
  ULSurfaceDefinitionCreateCallback create;
  ULSurfaceDefinitionDestroyCallback destroy;
  ULSurfaceDefinitionGetWidthCallback get_width;
  ULSurfaceDefinitionGetHeightCallback get_height;
  ULSurfaceDefinitionGetRowBytesCallback get_row_bytes;
  ULSurfaceDefinitionGetSizeCallback get_size;
  ULSurfaceDefinitionLockPixelsCallback lock_pixels;
  ULSurfaceDefinitionUnlockPixelsCallback unlock_pixels;
  ULSurfaceDefinitionResizeCallback resize;
} ULSurfaceDefinition;

/******************************************************************************
 * File System
 *****************************************************************************/

///
/// File Handle type used as unique ID for opened files.
///
#if defined(__WIN32__) || defined(_WIN32)
typedef size_t ULFileHandle;
#else
typedef int ULFileHandle;
#endif

///
/// Handle used to denote an invalid file.
///
const ULFileHandle ULInvalidFileHandle = (ULFileHandle)-1;

///
/// The callback invoked when the FileSystem wants to check if a file path
/// exists, return true if it exists.
///
typedef bool
(*ULFileSystemFileExistsCallback) (ULString path);

///
/// The callback invoked when the FileSystem wants to get the file size of a
/// previously opened file, store result in 'result'. Return true on success.
///
typedef bool
(*ULFileSystemGetFileSizeCallback) (ULFileHandle handle, long long* result);

///
/// The callback invoked when the FileSystem wants to get the mime type of a
/// file (eg "text/html"), store result in 'result'. Return true on success.
///
typedef bool
(*ULFileSystemGetFileMimeTypeCallback) (ULString path, ULString result);

///
/// The callback invoked when the FileSystem wants to open a file path for
/// reading or writing. Return file handle on success, or ULInvalidFileHandle
/// on failure.
///
/// @NOTE:  As of this writing (v1.2), this function is only used for reading.
///
typedef ULFileHandle
(*ULFileSystemOpenFileCallback) (ULString path, bool open_for_writing);

///
/// The callback invoked when the FileSystem wants to close a previously-opened
/// file.
///
typedef void
(*ULFileSystemCloseFileCallback) (ULFileHandle handle);

///
/// The callback invoked when the FileSystem wants to read from a
/// currently-opened file, return number of bytes read or -1 on failure.
///
typedef long long
(*ULFileSystemReadFromFileCallback) (ULFileHandle handle, char* data, long long length);

typedef struct {
  ULFileSystemFileExistsCallback file_exists;
  ULFileSystemGetFileSizeCallback get_file_size;
  ULFileSystemGetFileMimeTypeCallback get_file_mime_type;
  ULFileSystemOpenFileCallback open_file;
  ULFileSystemCloseFileCallback close_file;
  ULFileSystemReadFromFileCallback read_from_file;
} ULFileSystem;

/******************************************************************************
 * Logger
 *****************************************************************************/

typedef enum {
  kLogLevel_Error = 0,
  kLogLevel_Warning,
  kLogLevel_Info
} ULLogLevel;

///
/// The callback invoked when the library wants to print a message to the log.
///
typedef void
(*ULLoggerLogMessageCallback) (ULLogLevel log_level, ULString message);

typedef struct {
  ULLoggerLogMessageCallback log_message;
} ULLogger;


/******************************************************************************
 * GPUDriver
 *****************************************************************************/

///
/// Render buffer description.
///
typedef struct {
  unsigned int texture_id;  // The backing texture for this RenderBuffer
  unsigned int width;       // The width of the RenderBuffer texture
  unsigned int height;      // The height of the RenderBuffer texture
  bool has_stencil_buffer;  // Currently unused, always false.
  bool has_depth_buffer;    // Currently unsued, always false.
} ULRenderBuffer;

///
/// @note  This pragma pack(push, 1) command is important! Vertex layouts
///	       should not be padded with any bytes.
///
#pragma pack(push, 1)

///
/// Vertex layout for path vertices.
///
/// (this struct's members aligned on single-byte boundaries)
///
typedef struct {
  float pos[2];
  unsigned char color[4];
  float obj[2];
} ULVertex_2f_4ub_2f;

///
/// Vertex layout for quad vertices.
///
/// (this struct's members aligned on single-byte boundaries)
///
typedef struct {
  float pos[2];
  unsigned char color[4];
  float tex[2];
  float obj[2];
  float data0[4];
  float data1[4];
  float data2[4];
  float data3[4];
  float data4[4];
  float data5[4];
  float data6[4];
} ULVertex_2f_4ub_2f_2f_28f;

///
/// End single-byte alignment.
///
#pragma pack(pop)

///
/// Vertex formats.
///
typedef enum {
  kVertexBufferFormat_2f_4ub_2f,
  kVertexBufferFormat_2f_4ub_2f_2f_28f,
} ULVertexBufferFormat;

///
/// Vertex buffer data.
///
typedef struct {
  ULVertexBufferFormat format;
  unsigned int size;
  unsigned char* data;
} ULVertexBuffer;

///
/// Vertex index type.
///
typedef unsigned int ULIndexType;

///
/// Vertex index buffer data.
///
typedef struct {
  unsigned int size;
  unsigned char* data;
} ULIndexBuffer;

///
/// Shader types, used with ULGPUState::shader_type
///
/// Each of these correspond to a vertex/pixel shader pair. You can find
/// stock shader code for these in the `shaders` folder of the AppCore repo.
///
typedef enum {
  kShaderType_Fill,      // Shader program for quad geometry
  kShaderType_FillPath,  // Shader program for path geometry
} ULShaderType;

///
/// Raw 4x4 matrix as an array of floats
///
typedef struct {
  float data[16];
} ULMatrix4x4;

///
/// 4-component float vector
///
typedef struct {
  float value[4];
} ULvec4;

///
/// GPU State description.
///
typedef struct {
  /// Viewport width in pixels
  unsigned int viewport_width;

  /// Viewport height in pixels
  unsigned int viewport_height;

  /// Transform matrix-- you should multiply this with the screen-space
  /// orthographic projection matrix then pass to the vertex shader.
  ULMatrix4x4 transform;

  /// Whether or not we should enable texturing for the current draw command.
  bool enable_texturing;

  /// Whether or not we should enable blending for the current draw command.
  /// If blending is disabled, any drawn pixels should overwrite existing.
  /// Mainly used so we can modify alpha values of the RenderBuffer during
  /// scissored clears.
  bool enable_blend;

  /// The vertex/pixel shader program pair to use for the current draw command.
  /// You should cast this to ShaderType to get the corresponding enum.
  unsigned char shader_type;

  /// The render buffer to use for the current draw command.
  unsigned int render_buffer_id;

  /// The texture id to bind to slot #1. (Will be 0 if none)
  unsigned int texture_1_id;

  /// The texture id to bind to slot #2. (Will be 0 if none)
  unsigned int texture_2_id;

  /// The texture id to bind to slot #3. (Will be 0 if none)
  unsigned int texture_3_id;

  /// The following four members are passed to the pixel shader via uniforms.
  float uniform_scalar[8];
  ULvec4 uniform_vector[8];
  unsigned char clip_size;
  ULMatrix4x4 clip[8];

  /// Whether or not scissor testing should be used for the current draw
  /// command.
  bool enable_scissor;

  /// The scissor rect to use for scissor testing (units in pixels)
  ULIntRect scissor_rect;
} ULGPUState;

///
/// Command types, used with ULCommand::command_type
///
typedef enum {
  kCommandType_ClearRenderBuffer,
  kCommandType_DrawGeometry,
} ULCommandType;

///
/// Command description.
///
typedef struct {
  unsigned char command_type;    // The type of command to dispatch.
  ULGPUState gpu_state;          // GPU state parameters for current command.

  /// The following members are only used with kCommandType_DrawGeometry
  unsigned int geometry_id;    // The geometry ID to bind
  unsigned int indices_count;  // The number of indices
  unsigned int indices_offset; // The index to start from
} ULCommand;

///
/// Command list, @see ULGPUDriverUpdateCommandList
typedef struct {
  unsigned int size;
  ULCommand* commands;
} ULCommandList;

///
/// The callback invoked when the GPUDriver will begin dispatching commands
/// (such as CreateTexture and UpdateCommandList) during the current call to
/// ulRender().
///
typedef void
(*ULGPUDriverBeginSynchronizeCallback) ();

///
/// The callback invoked when the GPUDriver has finished dispatching commands.
/// during the current call to ulRender().
///
typedef void
(*ULGPUDriverEndSynchronizeCallback) ();

///
/// The callback invoked when the GPUDriver wants to get the next available
/// texture ID.
///
typedef unsigned int
(*ULGPUDriverNextTextureIdCallback) ();

///
/// The callback invoked when the GPUDriver wants to create a texture with a
/// certain ID and optional bitmap.
///
/// **NOTE**: If the Bitmap is empty (ulBitmapIsEmpty), then a RTT Texture
///           should be created instead. This will be used as a backing
///           texture for a new RenderBuffer.
///
typedef void
(*ULGPUDriverCreateTextureCallback) (unsigned int texture_id,
                                     ULBitmap bitmap);

///
/// The callback invoked when the GPUDriver wants to update an existing non-RTT
/// texture with new bitmap data.
///
typedef void
(*ULGPUDriverUpdateTextureCallback) (unsigned int texture_id,
                                     ULBitmap bitmap);

///
/// The callback invoked when the GPUDriver wants to destroy a texture.
///
typedef void
(*ULGPUDriverDestroyTextureCallback) (unsigned int texture_id);

///
/// The callback invoked when the GPUDriver wants to generate the next
/// available render buffer ID.
///
typedef unsigned int
(*ULGPUDriverNextRenderBufferIdCallback) ();

///
/// The callback invoked when the GPUDriver wants to create a render buffer
/// with certain ID and buffer description.
///
typedef void
(*ULGPUDriverCreateRenderBufferCallback) (unsigned int render_buffer_id,
                                          ULRenderBuffer buffer);

///
/// The callback invoked when the GPUDriver wants to destroy a render buffer
///
typedef void
(*ULGPUDriverDestroyRenderBufferCallback) (unsigned int render_buffer_id);

///
/// The callback invoked when the GPUDriver wants to generate the next
/// available geometry ID.
///
typedef unsigned int
(*ULGPUDriverNextGeometryIdCallback) ();

///
/// The callback invoked when the GPUDriver wants to create geometry with
/// certain ID and vertex/index data.
///
typedef void
(*ULGPUDriverCreateGeometryCallback) (unsigned int geometry_id,
                                      ULVertexBuffer vertices,
                                      ULIndexBuffer indices);

///
/// The callback invoked when the GPUDriver wants to update existing geometry
/// with new vertex/index data.
///
typedef void
(*ULGPUDriverUpdateGeometryCallback) (unsigned int geometry_id,
                                      ULVertexBuffer vertices,
                                      ULIndexBuffer indices);

///
/// The callback invoked when the GPUDriver wants to destroy geometry.
///
typedef void
(*ULGPUDriverDestroyGeometryCallback) (unsigned int geometry_id);

///
/// The callback invoked when the GPUDriver wants to update the command list
/// (you should copy the commands to your own structure).
///
typedef void
(*ULGPUDriverUpdateCommandListCallback) (ULCommandList list);

typedef struct {
  ULGPUDriverBeginSynchronizeCallback begin_synchronize;
  ULGPUDriverEndSynchronizeCallback end_synchronize;
  ULGPUDriverNextTextureIdCallback next_texture_id;
  ULGPUDriverCreateTextureCallback create_texture;
  ULGPUDriverUpdateTextureCallback update_texture;
  ULGPUDriverDestroyTextureCallback destroy_texture;
  ULGPUDriverNextRenderBufferIdCallback next_render_buffer_id;
  ULGPUDriverCreateRenderBufferCallback create_render_buffer;
  ULGPUDriverDestroyRenderBufferCallback destroy_render_buffer;
  ULGPUDriverNextGeometryIdCallback next_geometry_id;
  ULGPUDriverCreateGeometryCallback create_geometry;
  ULGPUDriverUpdateGeometryCallback update_geometry;
  ULGPUDriverDestroyGeometryCallback destroy_geometry;
  ULGPUDriverUpdateCommandListCallback update_command_list;
} ULGPUDriver;

///
/// Sets up an orthographic projection matrix with a certain viewport width
/// and height, multiplies it by 'transform', and returns the result.
///
/// This should be used to calculate the model-view projection matrix for the
/// vertex shaders using the current ULGPUState.
///
/// The 'flip_y' can be optionally used to flip the Y coordinate-space.
/// (Usually flip_y == true for OpenGL)
///
ULExport ULMatrix4x4 ulApplyProjection(ULMatrix4x4 transform,
                                       float viewport_width,
                                       float viewport_height,
                                       bool flip_y);

/******************************************************************************
 * Clipboard
 *****************************************************************************/

///
/// The callback invoked when the library wants to clear the system's
/// clipboard.
///
typedef void
(*ULClipboardClearCallback) ();

///
/// The callback invoked when the library wants to read from the system's
/// clipboard.
///
/// You should store the result (if any) in 'result'.
///
typedef void
(*ULClipboardReadPlainTextCallback) (ULString result);

///
/// The callback invoked when the library wants to write to the system's
/// clipboard.
///
typedef void
(*ULClipboardWritePlainTextCallback) (ULString text);

typedef struct {
  ULClipboardClearCallback clear;
  ULClipboardReadPlainTextCallback read_plain_text;
  ULClipboardWritePlainTextCallback write_plain_text;
} ULClipboard;

/******************************************************************************
 * Platform
 *****************************************************************************/

///
/// Set a custom Logger implementation.
///
/// This is used to log debug messages to the console or to a log file.
///
/// You should call this before ulCreateRenderer() or ulCreateApp().
///
/// @note  ulCreateApp() will use the default logger if you never call this.
///
/// @note  If you're not using ulCreateApp(), (eg, using ulCreateRenderer())
///        you can still use the default logger by calling
///        ulEnableDefaultLogger() (@see <AppCore/CAPI.h>)
///
ULExport void ulPlatformSetLogger(ULLogger logger);

///
/// Set a custom FileSystem implementation.
///          
/// This is used for loading File URLs (eg, <file:///page.html>). If you don't
/// call this, and are not using ulCreateApp() or ulEnablePlatformFileSystem(), 
/// you will not be able to load any File URLs.
///
/// You should call this before ulCreateRenderer() or ulCreateApp().
///
/// @note  ulCreateApp() will use the default platform file system if you never
///        call this.
///
/// @note  If you're not using ulCreateApp(), (eg, using ulCreateRenderer())
///        you can still use the default platform file system by calling
///        ulEnablePlatformFileSystem() (@see <AppCore/CAPI.h>)
///
ULExport void ulPlatformSetFileSystem(ULFileSystem file_system);

///
/// Set a custom Surface implementation.
///
/// This can be used to wrap a platform-specific GPU texture, Windows DIB,
/// macOS CGImage, or any other pixel buffer target for display on screen.
///
/// By default, the library uses a bitmap surface for all surfaces but you can
/// override this by providing your own surface definition here.
///
/// You should call this before ulCreateRenderer() or ulCreateApp().
///
ULExport void ulPlatformSetSurfaceDefinition(ULSurfaceDefinition surface_definition);

///
/// Set a custom GPUDriver implementation.
///
/// This should be used if you have enabled the GPU renderer in the Config and
/// are using ulCreateRenderer() (which does not provide its own GPUDriver
/// implementation).
///
/// The GPUDriver interface is used by the library to dispatch GPU calls to
/// your native GPU context (eg, D3D11, Metal, OpenGL, Vulkan, etc.) There
/// are reference implementations for this interface in the AppCore repo.
///
/// You should call this before ulCreateRenderer().
///
ULExport void ulPlatformSetGPUDriver(ULGPUDriver gpu_driver);

///
/// Set a custom Clipboard implementation.
///
/// This should be used if you are using ulCreateRenderer() (which does not
/// provide its own clipboard implementation).
///
/// The Clipboard interface is used by the library to make calls to the
/// system's native clipboard (eg, cut, copy, paste).
///
/// You should call this before ulCreateRenderer().
///
ULExport void ulPlatformSetClipboard(ULClipboard clipboard);

#ifdef __cplusplus
}
#endif

#endif // ULTRALIGHT_CAPI_H

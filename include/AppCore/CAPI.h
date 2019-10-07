///
/// @file CAPI.h
///
/// @brief The C-language API for AppCore
///
/// @author
///
/// This file is a part of Ultralight, a fast, lightweight, HTML UI engine
///
/// Website: <http://ultralig.ht>
///
/// Copyright (C) 2019 Ultralight, Inc. All rights reserved.
///
#ifndef APPCORE_CAPI_H
#define APPCORE_CAPI_H

#include <Ultralight/CAPI.h>

#if defined(__WIN32__) || defined(_WIN32)
#  if defined(APPCORE_IMPLEMENTATION)
#    define ACExport __declspec(dllexport)
#  else
#    define ACExport __declspec(dllimport)
#  endif
#else
#  define ACExport __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct C_Settings* ULSettings;
typedef struct C_App* ULApp;
typedef struct C_Window* ULWindow;
typedef struct C_Monitor* ULMonitor;
typedef struct C_Overlay* ULOverlay;

///
/// Window creation flags. @see Window::Create
///
typedef enum {
  kWindowFlags_Borderless = 1 << 0,
  kWindowFlags_Titled = 1 << 1,
  kWindowFlags_Resizable = 1 << 2,
  kWindowFlags_Maximizable = 1 << 3,
} ULWindowFlags;

///
/// Create settings with default values (see <AppCore/App.h>).
///
ACExport ULSettings ulCreateSettings();

///
/// Destroy settings.
///
ACExport void ulDestroySettings(ULSettings settings);

///
/// Set the root file path for our file system, you should set this to the
/// relative path where all of your app data is.
///
/// This will be used to resolve all file URLs, eg file:///page.html
///
/// @note  By default, on macOS we use the app bundle's @resource_path,
///        on all other platforms we use the "./assets/" directory relative
///        to the executable's directory.
///
ACExport void ulSettingsSetFileSystemPath(ULSettings settings, ULString path);

///
/// Set whether or not we should load and compile shaders from the file system
/// (eg, from the /shaders/ path, relative to file_system_path).
///
/// If this is false (the default), we will instead load pre-compiled shaders
/// from memory which speeds up application startup time.
///
ACExport void ulSettingsSetLoadShadersFromFileSystem(ULSettings settings,
                                                     bool enabled);

///
/// Create the App singleton.
///
/// @param  settings  Settings to customize App runtime behavior. You can pass
///                   NULL for this parameter to use default settings.
///
/// @param  config  Config options for the Ultralight renderer. You can pass
///                 NULL for this parameter to use default config.
///
/// @note  You should only create one of these per application lifetime.
///        
/// @note  Certain Config options may be overridden during App creation,
///        most commonly Config::face_winding and Config::device_scale_hint.
///
ACExport ULApp ulCreateApp(ULSettings settings, ULConfig config);

///
/// Destroy the App instance.
///
ACExport void ulDestroyApp(ULApp app);

///
/// Set the main window, you must set this before calling ulAppRun.
///
/// @param  window  The window to use for all rendering.
///
/// @note  We currently only support one Window per App, this will change
///        later once we add support for multiple driver instances.
///
ACExport void ulAppSetWindow(ULApp app, ULWindow window);

///
/// Get the main window.
///
ACExport ULWindow ulAppGetWindow(ULApp app);

typedef void
(*ULUpdateCallback) (void* user_data);

///
/// Set a callback for whenever the App updates. You should update all app
/// logic here.
///
/// @note  This event is fired right before the run loop calls
///        Renderer::Update and Renderer::Render.
///
ACExport void ulAppSetUpdateCallback(ULApp app, ULUpdateCallback callback,
                                     void* user_data);

///
/// Whether or not the App is running.
///
ACExport bool ulAppIsRunning(ULApp app);

///
/// Get the main monitor (this is never NULL).
///
/// @note  We'll add monitor enumeration later.
///
ACExport ULMonitor ulAppGetMainMonitor(ULApp app);

///
/// Get the underlying Renderer instance.
///
ACExport ULRenderer ulAppGetRenderer(ULApp app);

///
/// Run the main loop, make sure to call ulAppSetWindow before calling this.
///
ACExport void ulAppRun(ULApp app);

///
/// Quit the application.
///
ACExport void ulAppQuit(ULApp app);

///
/// Get the monitor's DPI scale (1.0 = 100%).
///
ACExport double ulMonitorGetScale(ULMonitor monitor);

///
/// Get the width of the monitor (in device coordinates).
///
ACExport unsigned int ulMonitorGetWidth(ULMonitor monitor);

///
/// Get the height of the monitor (in device coordinates).
///
ACExport unsigned int ulMonitorGetHeight(ULMonitor monitor);

///
/// Create a new Window.
///
/// @param  monitor       The monitor to create the Window on.
///
/// @param  width         The width (in device coordinates).
///
/// @param  height        The height (in device coordinates).
///
/// @param  fullscreen    Whether or not the window is fullscreen.
///
/// @param  window_flags  Various window flags.
///
ACExport ULWindow ulCreateWindow(ULMonitor monitor, unsigned int width,
	                               unsigned int height, bool fullscreen,
                                 unsigned int window_flags);

///
/// Destroy a Window.
///
ACExport void ulDestroyWindow(ULWindow window);

typedef void
(*ULCloseCallback) (void* user_data);

///
/// Set a callback to be notified when a window closes.
///
ACExport void ulWindowSetCloseCallback(ULWindow window,
                                       ULCloseCallback callback,
                                       void* user_data);

typedef void
(*ULResizeCallback) (void* user_data, unsigned int width, unsigned int height);

///
/// Set a callback to be notified when a window resizes
/// (parameters are passed back in device coordinates).
///
ACExport void ulWindowSetResizeCallback(ULWindow window,
                                        ULResizeCallback callback,
                                        void* user_data);

///
/// Get window width (in device coordinates).
///
ACExport unsigned int ulWindowGetWidth(ULWindow window);

///
/// Get window height (in device coordinates).
///
ACExport unsigned int ulWindowGetHeight(ULWindow window);

///
/// Get whether or not a window is fullscreen.
///
ACExport bool ulWindowIsFullscreen(ULWindow window);

///
/// Get the DPI scale of a window.
///
ACExport double ulWindowGetScale(ULWindow window);

///
/// Set the window title.
///
ACExport void ulWindowSetTitle(ULWindow window, const char* title);

///
/// Set the cursor for a window.
///
ACExport void ulWindowSetCursor(ULWindow window, ULCursor cursor);

///
/// Close a window.
///
ACExport void ulWindowClose(ULWindow window);

///
/// Convert device coordinates to pixels using the current DPI scale.
///
ACExport int ulWindowDeviceToPixel(ULWindow window, int val);

///
/// Convert pixels to device coordinates using the current DPI scale.
///
ACExport int ulWindowPixelsToDevice(ULWindow window, int val);

///
/// Create a new Overlay.
///
/// @param  window  The window to create the Overlay in. (we currently only
///                 support one window per application)
///
/// @param  width   The width in device coordinates.
///
/// @param  height  The height in device coordinates.
///
/// @param  x       The x-position (offset from the left of the Window), in
///                 device coordinates.
///
/// @param  y       The y-position (offset from the top of the Window), in
///                 device coordinates.
///
/// @note  Each Overlay is essentially a View and an on-screen quad. You should
///        create the Overlay then load content into the underlying View.
///
ACExport ULOverlay ulCreateOverlay(ULWindow window, unsigned int width,
                                   unsigned int height, int x, int y);

///
/// Destroy an overlay.
///
ACExport void ulDestroyOverlay(ULOverlay overlay);

///
/// Get the underlying View.
///
ACExport ULView ulOverlayGetView(ULOverlay overlay);

///
/// Get the width (in device coordinates).
///
ACExport unsigned int ulOverlayGetWidth(ULOverlay overlay);

///
/// Get the height (in device coordinates).
///
ACExport unsigned int ulOverlayGetHeight(ULOverlay overlay);

///
/// Get the x-position (offset from the left of the Window), in device
/// coordinates.
///
ACExport int ulOverlayGetX(ULOverlay overlay);

///
/// Get the y-position (offset from the top of the Window), in device
/// coordinates.
///
ACExport int ulOverlayGetY(ULOverlay overlay);

///
/// Move the overlay to a new position (in device coordinates).
///
ACExport void ulOverlayMoveTo(ULOverlay overlay, int x, int y);

///
/// Resize the overlay (and underlying View), dimensions should be
/// specified in device coordinates.
///
ACExport void ulOverlayResize(ULOverlay overlay, unsigned int width,
                              unsigned int height);

///
/// Whether or not the overlay is hidden (not drawn).
///
ACExport bool ulOverlayIsHidden(ULOverlay overlay);

///
/// Hide the overlay (will no longer be drawn).
///
ACExport void ulOverlayHide(ULOverlay overlay);

///
/// Show the overlay.
///
ACExport void ulOverlayShow(ULOverlay overlay);

///
/// Whether or not an overlay has keyboard focus.
///
ACExport bool ulOverlayHasFocus(ULOverlay overlay);

///
/// Grant this overlay exclusive keyboard focus.
///
ACExport void ulOverlayFocus(ULOverlay overlay);

///
/// Remove keyboard focus.
///
ACExport void ulOverlayUnfocus(ULOverlay overlay);

#ifdef __cplusplus
}
#endif

#endif // APPCORE_CAPI_H

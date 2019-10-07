///
/// @file App.h
///
/// @brief The header for the App class.
///
/// @author
///
/// This file is a part of Ultralight, a fast, lightweight, HTML UI engine
///
/// Website: <http://ultralig.ht>
///
/// Copyright (C) 2019 Ultralight, Inc. All rights reserved.
///
#pragma once
#include "Defines.h"
#include <Ultralight/RefPtr.h>
#include <Ultralight/Renderer.h>
#include <Ultralight/platform/Config.h>

namespace ultralight {

class Monitor;
class Window;

///
/// Interface for all App-related events. @see App::set_listener
///
class AExport AppListener {
public:
  virtual ~AppListener() {}

  ///
  /// Called whenever the App updates. You should update all app logic here.
  ///
  /// @note  This event is fired right before the run loop calls
  ///        Renderer::Update and Renderer::Render.
  ///
  virtual void OnUpdate() {}
};

///
/// App-specific settings.
///
struct AExport Settings {
  ///
  /// The root file path for our file system. You should set this to the
  /// relative path where all of your app data is.
  ///
  /// This will be used to resolve all file URLs, eg file:///page.html
  ///
  /// @note  By default, on macOS we use the app bundle's @resource_path,
  ///        on all other platforms we use the "./assets/" directory relative
  ///        to the executable's directory.
  ///
#ifdef __APPLE__
  String file_system_path = "@resource_path";
#else
  String file_system_path = "./assets/";
#endif

  ///
  /// Whether or not we should load and compile shaders from the file system
  /// (eg, from the /shaders/ path, relative to file_system_path).
  ///
  /// If this is false (the default), we will instead load pre-compiled shaders
  /// from memory which speeds up application startup time.
  ///
  bool load_shaders_from_file_system = false;
};

///
/// Main application class.
///
class AExport App : public RefCounted {
public:
  ///
  /// Create the App singleton.
  ///
  /// @param  settings  Settings to customize App runtime behavior.
  ///
  /// @param  config  Config options for the Ultralight renderer.
  ///
  /// @return  Returns a ref-pointer to the created App instance.
  ///
  /// @note  You should only create one of these per application lifetime.
  ///
  /// @note  Certain Config options may be overridden during App creation,
  ///        most commonly Config::face_winding and Config::device_scale_hint.
  ///
  static Ref<App> Create(Settings settings = Settings(), Config config = Config());

  ///
  /// Get the App singleton.
  ///
  static App* instance();

  ///
  /// Get the settings this App was created with.
  ///
  virtual const Settings& settings() const = 0;

  ///
  /// Set the main window. You must set this before calling Run.
  ///
  /// @param  window  The window to use for all rendering.
  ///
  /// @note  We currently only support one Window per App, this will change
  ///        later once we add support for multiple driver instances.
  ///
  virtual void set_window(Ref<Window> window) = 0;

  ///
  /// Get the main window.
  ///
  virtual RefPtr<Window> window() = 0;

  ///
  /// Set an AppListener to receive callbacks for app-related events.
  ///
  /// @note  Ownership remains with the caller.
  ///
  virtual void set_listener(AppListener* listener) = 0;

  ///
  /// Get the AppListener, if any.
  ///
  virtual AppListener* listener() = 0;

  ///
  /// Whether or not the App is running.
  ///
  virtual bool is_running() const = 0;

  ///
  /// Get the main monitor (this is never NULL).
  ///
  /// @note  We'll add monitor enumeration later.
  ///
  virtual Monitor* main_monitor() = 0;

  ///
  /// Get the underlying Renderer instance.
  ///
  virtual Ref<Renderer> renderer() = 0;

  ///
  /// Run the main loop.
  ///
  /// @note  Make sure to call set_window before calling this.
  ///
  virtual void Run() = 0;

  ///
  /// Quit the application.
  ///
  virtual void Quit() = 0;

protected:
  virtual ~App();
};

}  // namespace ultralight

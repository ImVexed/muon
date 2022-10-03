///
/// @file Renderer.h
///
/// @brief The header for the Renderer class.
///
/// @author
///
/// This file is a part of Ultralight, a fast, lightweight, HTML UI engine
///
/// Website: <http://ultralig.ht>
///
/// Copyright (C) 2020 Ultralight, Inc. All rights reserved.
///
#pragma once
#include <Ultralight/Defines.h>
#include <Ultralight/RefPtr.h>
#include <Ultralight/Session.h>
#include <Ultralight/View.h>

namespace ultralight {

///
/// @brief  This singleton manages the lifetime of all Views (@see View) and
///         coordinates painting, network requests, and event dispatch.
///
/// @note  You don't have to create this instance directly if you use the
///        AppCore API. The App class will automatically create a Renderer and
///        perform all rendering within its run loop. @see App::Create
///
class UExport Renderer : public RefCounted {
public:
  ///
  /// Create the Ultralight Renderer directly.
  ///
  /// Unlike App::Create(), this does not use any native windows for drawing
  /// and allows you to manage your own runloop and painting. This method is
  /// recommended for those wishing to integrate the library into a game.
  ///
  /// You should set up your Platform config, file-system, font loader, 
  /// and surface-factories/gpu-drivers before calling this function. 
  /// (@see <Ultralight/Platform.h>)
  ///
  /// At a minimum, you will need to define a FontLoader ahead of time or this
  /// call will fail. You can use the platform's native FontLoader by calling:
  /// <pre>
  ///   /// This function is defined in <AppCore/Platform.h>
  ///   Platform::instance().set_font_loader(GetPlatformFontLoader());
  /// </pre>
  ///
  /// @note  You should only create one Renderer per application lifetime.
  ///
  /// @note: You should not call this if you are using App::Create(), it
  ///        creates its own renderer and provides default implementations for
  ///        various platform handlers automatically.
  ///
  /// @return  Renderer is ref-counted. This method returns a ref-pointer
  ///          to a new instance, you should store it in a RefPtr<> to keep
  ///          the instance alive.
  ///
  static Ref<Renderer> Create();

  ///
  /// Create a Session to store local data in (such as cookies, local storage,
  /// application cache, indexed db, etc).
  ///
  /// @note  A default, persistent Session is already created for you. You
  ///        only need to call this if you want to create private, in-memory
  ///        session or use a separate session for each View.
  ///
  /// @param  is_persistent  Whether or not to store the session on disk.
  ///                        Persistent sessions will be written to the path
  ///                        set in Config::cache_path
  ///
  /// @param  name  A unique name for this session, this will be used to
  ///               generate a unique disk path for persistent sessions.
  ///
  virtual Ref<Session> CreateSession(bool is_persistent, const String& name) = 0;
  
  ///
  /// Get the default Session. This session is persistent (backed to disk) and
  /// has the name "default".
  ///
  virtual Ref<Session> default_session() = 0;

  ///
  /// Create a new View.
  ///
  /// @param  width   The initial width, in pixels.
  /// 
  /// @param  height  The initial height, in pixels.
  ///
  /// @param  transparent  Whether or not the view background is transparent.
  ///
  /// @param  session  The session to store local data in. Pass a nullptr to
  ///                  use the default session.
  ///
  /// @return  Returns a ref-pointer to a new View instance. You should assign
  ///          it to either a Ref<View> (non-nullable) or RefPtr<View>
  ///          (nullable).
  ///
  virtual Ref<View> CreateView(uint32_t width, uint32_t height,
	                             bool transparent, RefPtr<Session> session,
                               bool force_cpu_renderer = false) = 0;

  ///
  /// Update timers and dispatch internal callbacks. You should call this often
  /// from your main application loop.
  ///
  virtual void Update() = 0;

  ///
  /// Render all active views to their respective render-targets/surfaces.
  ///
  /// You should call this once per frame (usually in synchrony with the
  /// monitor's refresh rate).
  ///
  /// @note  Views are only repainted if they actually need painting.
  ///
  virtual void Render() = 0;

  ///
  /// Attempt to release as much memory as possible. Don't call this from any
  /// callbacks or driver code.
  ///
  virtual void PurgeMemory() = 0;

  ///
  /// Print detailed memory usage statistics to the log.
  /// (@see Platform::set_logger())
  ///
  virtual void LogMemoryUsage() = 0;

protected:
  virtual ~Renderer();
};

} // namespace ultralight

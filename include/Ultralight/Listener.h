///
/// @file Listener.h
///
/// @brief The header for View listener interfaces.
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
#include <Ultralight/String.h>
#include <Ultralight/RefPtr.h>
#include <Ultralight/Geometry.h>

namespace ultralight {

class View;

///
/// MessageSource types, @see ViewListener::OnAddConsoleMessage
///
enum MessageSource {
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
};

///
/// MessageLevel types, @see ViewListener::OnAddConsoleMessage
///
enum MessageLevel {
  kMessageLevel_Log = 1,
  kMessageLevel_Warning = 2,
  kMessageLevel_Error = 3,
  kMessageLevel_Debug = 4,
  kMessageLevel_Info = 5,
};

///
/// Cursor types, @see ViewListener::OnChangeCursor
///
enum Cursor {
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
};

///
/// @brief  Interface for View-related events
///
/// @note   For more info @see View::set_view_listener
///
class UExport ViewListener {
public:
  virtual ~ViewListener() {}

  ///
  /// Called when the page title changes
  ///
  virtual void OnChangeTitle(ultralight::View* caller,
                             const String& title) {}

  ///
  /// Called when the page URL changes
  ///
  virtual void OnChangeURL(ultralight::View* caller,
                           const String& url) {}

  ///
  /// Called when the tooltip changes (usually as result of a mouse hover)
  ///
  virtual void OnChangeTooltip(ultralight::View* caller,
                               const String& tooltip) {}

  ///
  /// Called when the mouse cursor changes
  ///
  virtual void OnChangeCursor(ultralight::View* caller,
                              Cursor cursor) {}

  ///
  /// Called when a message is added to the console (useful for errors / debug)
  ///
  virtual void OnAddConsoleMessage(ultralight::View* caller,
                                   MessageSource source,
                                   MessageLevel level,
                                   const String& message,
                                   uint32_t line_number,
                                   uint32_t column_number,
                                   const String& source_id) {}

  ///
  /// Called when the page wants to create a new View.
  ///
  /// This is usually the result of a user clicking a link with target="_blank"
  /// or by JavaScript calling window.open(url).
  ///
  /// To allow creation of these new Views, you should create a new View in
  /// this callback (eg, Renderer::CreateView()), resize it to your container,
  /// and return it. You are responsible for displaying the returned View.
  ///
  /// @param  caller      The View that called this event.
  ///
  /// @param  opener_url  The URL of the page that initiated this request.
  ///
  /// @param  target_url  The URL that the new View will navigate to.
  ///
  /// @param  is_popup    Whether or not this was triggered by window.open().
  ///
  /// @param  popup_rect  Popups can optionally request certain dimensions and
  ///                     coordinates via window.open(). You can choose to
  ///                     respect these or not by resizing/moving the View to
  ///                     this rect.
  ///
  /// @return  Returns a RefPtr<> to a created View to use to satisfy the
  ///          the request (or return nullptr if you want to block the action).
  ///
  virtual RefPtr<View> OnCreateChildView(ultralight::View* caller,
                                         const String& opener_url,
                                         const String& target_url,
                                         bool is_popup,
                                         const IntRect& popup_rect);
};

///
/// @brief  Interface for Load-related events
///
/// @note   For more info @see View::set_load_listener
///
class UExport LoadListener {
public:
  virtual ~LoadListener() {}

  ///
  /// Called when the page begins loading a new URL into a frame.
  ///
  /// @param  frame_id       A unique ID for the frame.
  ///
  /// @param  is_main_frame  Whether or not this is the main frame.
  ///
  /// @param  url            The URL for the load.
  ///
  virtual void OnBeginLoading(ultralight::View* caller,
                              uint64_t frame_id,
                              bool is_main_frame,
                              const String& url) {}

  ///
  /// Called when the page finishes loading a URL into a frame.
  ///
  /// @param  frame_id       A unique ID for the frame.
  ///
  /// @param  is_main_frame  Whether or not this is the main frame.
  ///
  /// @param  url            The URL for the load.
  ///
  virtual void OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const String& url) {}

  ///
  /// Called when an error occurs while loading a URL into a frame.
  ///
  /// @param  frame_id       A unique ID for the frame.
  ///
  /// @param  is_main_frame  Whether or not this is the main frame.
  ///
  /// @param  url            The URL for the load.
  ///
  /// @param  description    A human-readable description of the error.
  ///
  /// @param  error_domain   The name of the module that triggered the error.
  ///
  /// @param  error_code     Internal error code generated by the module.
  ///
  virtual void OnFailLoading(ultralight::View* caller,
                             uint64_t frame_id,
                             bool is_main_frame,
                             const String& url,
                             const String& description,
                             const String& error_domain,
                             int error_code) {}

  ///
  /// Called when the JavaScript window object is reset for a new page load.
  ///
  /// This is called before any scripts are executed on the page and is the
  /// earliest time to setup any initial JavaScript state or bindings.
  ///
  /// The document is not guaranteed to be loaded/parsed at this point. If
  /// you need to make any JavaScript calls that are dependent on DOM elements
  /// or scripts on the page, use OnDOMReady instead.
  ///
  /// The window object is lazily initialized (this will not be called on pages
  /// with no scripts).
  ///
  /// @param  frame_id       A unique ID for the frame.
  ///
  /// @param  is_main_frame  Whether or not this is the main frame.
  ///
  /// @param  url            The URL for the load.
  ///
  virtual void OnWindowObjectReady(ultralight::View* caller,
                                   uint64_t frame_id,
                                   bool is_main_frame,
                                   const String& url) {}

  ///
  /// Called when all JavaScript has been parsed and the document is ready.
  ///
  /// This is the best time to make any JavaScript calls that are dependent on
  /// DOM elements or scripts on the page.
  ///
  /// @param  frame_id       A unique ID for the frame.
  ///
  /// @param  is_main_frame  Whether or not this is the main frame.
  ///
  /// @param  url            The URL for the load.
  ///
  virtual void OnDOMReady(ultralight::View* caller,
                          uint64_t frame_id,
                          bool is_main_frame,
                          const String& url) {}

  ///
  /// Called when the session history (back/forward state) is modified.
  ///
  virtual void OnUpdateHistory(ultralight::View* caller) {}

};

}  // namespace ultralight

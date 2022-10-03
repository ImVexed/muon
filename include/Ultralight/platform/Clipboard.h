///
/// @file Clipboard.h
///
/// @brief The header for the Clipboard interface.
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
#include <Ultralight/String16.h>

namespace ultralight {

///
/// @brief  Clipboard interface.
///          
/// This is used for reading and writing data to the platform Clipboard.
///
/// AppCore automatically provides a platform-specific implementation of this
/// that cuts/copies/pastes to the OS clipboard when you call App::Create().
///
/// If you are using Renderer::Create() instead, you will need to provide your
/// own implementation of this. @see Platform::set_clipboard().
///
class UExport Clipboard {
public:
  virtual ~Clipboard();

  ///
  /// Clear the clipboard.
  ///
  virtual void Clear() = 0;

  ///
  /// Read plain text from the clipboard
  ///
  virtual String16 ReadPlainText() = 0;

  ///
  /// Write plain text to the clipboard.
  ///
  virtual void WritePlainText(const String16& text) = 0;
};

}  // namespace ultralight

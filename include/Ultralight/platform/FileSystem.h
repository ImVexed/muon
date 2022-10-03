///
/// @file FileSystem.h
///
/// @brief The header for the FileSystem interface.
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
/// File Handle type used as unique ID for opened files.
///
#if defined(__WIN32__) || defined(_WIN32)
typedef size_t FileHandle;
#else
typedef int FileHandle;
#endif

///
/// Handle used to denote an invalid file.
///
const FileHandle invalidFileHandle = (FileHandle)-1;

///
/// @brief  FileSystem interface.
///          
/// This is used for loading File URLs (eg, <file:///page.html>).
///
/// You can provide the library with your own FileSystem implementation so that
/// file assets are loaded from your own pipeline (useful if you would like
/// to encrypt/compress your file assets or ship it in a custom format).
///
/// AppCore automatically provides a platform-specific implementation of this
/// that loads files from a local directory when you call App::Create().
///
/// If you are using Renderer::Create() instead, you will need to provide your
/// own implementation via `Platform::instance().set_file_system(). For
/// convenience, you can still use AppCore's file system implementation--
/// see the helper functions defined in <AppCore/Platform.h>.
///
/// To provide your own custom FileSystem implementation, you should inherit
/// from this class, handle the virtual member functions, and then pass an
/// instance of your class to `Platform::instance().set_file_system()` before
/// calling Renderer::Create() or App::Create().
///
class UExport FileSystem {
public:
  virtual ~FileSystem();

  ///
  /// Check if file path exists, return true if exists.
  ///
  virtual bool FileExists(const String16& path) = 0;

  ///
  /// Get file size of previously opened file, store result in 'result'. Return true on success.
  ///
  virtual bool GetFileSize(FileHandle handle, int64_t& result) = 0;

  ///
  /// Get file mime type (eg "text/html"), store result in 'result'. Return true on success.
  ///
  virtual bool GetFileMimeType(const String16& path, String16& result) = 0;

  ///
  /// Open file path for reading or writing. Return file handle on success, or invalidFileHandle on failure.
  ///
  /// @NOTE:  As of this writing (v1.2), this function is only used for reading.
  ///
  virtual FileHandle OpenFile(const String16& path, bool open_for_writing) = 0;

  ///
  /// Close previously-opened file.
  ///
  virtual void CloseFile(FileHandle& handle) = 0;

  ///
  /// Read from currently-opened file, return number of bytes read or -1 on failure.
  ///
  virtual int64_t ReadFromFile(FileHandle handle, char* data, int64_t length) = 0;
};

}  // namespace ultralight

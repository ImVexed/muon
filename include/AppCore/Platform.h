///
/// @file Platform.h
///
/// @brief The header for the Platform helpers.
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
#include "Defines.h"
#include <Ultralight/String.h>
#include <Ultralight/platform/FontLoader.h>
#include <Ultralight/platform/FileSystem.h>
#include <Ultralight/platform/Logger.h>

namespace ultralight {

///
/// Get the native font loader for the current platform.
///
/// @note  This singleton is owned by the library, do not destroy it.
///
AExport FontLoader* GetPlatformFontLoader();

///
/// Get the native file system for the current platform, creating it if it
/// doesn't exist using the base directory provided.
///
/// This is used to load data for file:/// URLs.
///
/// @param  baseDir  An base file path that will be used to resolve relative
///                  file paths. You can optionally specify "@resource_path"
///                  on macOS to use the app bundle's resource path.
///
/// @note  This singleton is owned by the library, do not destroy it.
///
AExport FileSystem* GetPlatformFileSystem(const String& baseDir);

///
/// Get the default logger (writes the log to a file on disk).
///
/// @param  logPath  A file path to write the log to.
///
/// @note  This singleton is owned by the library, do not destroy it.
///
AExport Logger* GetDefaultLogger(const String& logPath);

}  // namespace ultralight

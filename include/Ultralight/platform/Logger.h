///
/// @file Logger.h
///
/// @brief The header for the Logger interface.
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
/// Log levels, used with Logger::LogMessage
///
enum LogLevel {
  kLogLevel_Error = 0,
  kLogLevel_Warning,
  kLogLevel_Info
};

///
/// @brief  Logger interface.
///          
/// This can be used to log debug messages to the console or to a log file.
///
/// This is intended to be implemented by users and defined before creating the
/// Renderer. @see Platform::set_file_system.
///
class UExport Logger {
public:
  virtual ~Logger();

  ///
  /// Called when the library wants to print a message to the log.
  ///
  virtual void LogMessage(LogLevel log_level, const String16& message) = 0;
};

}  // namespace ultralight

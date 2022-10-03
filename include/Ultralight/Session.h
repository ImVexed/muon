///
/// @file Session.h
///
/// @brief The header for the Session class.
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
#include <Ultralight/String.h>

namespace ultralight {

///
/// @brief A Session stores local data such as cookies, local storage,
///        and application cache for one or more Views. 
///
/// @see   Renderer::CreateSession
///
class UExport Session : public RefCounted {
public:
  ///
  /// Whether or not this session is written to disk.
  ///
  virtual bool is_persistent() const = 0;

  ///
  /// A unique name identifying this session.
  ///
  virtual String name() const = 0;

  ///
  /// A unique numeric ID identifying this session.
  ///
  virtual uint64_t id() const = 0;

  ///
  /// The disk path of this session (only valid for persistent sessions).
  ///
  virtual String disk_path() const = 0;

protected:
  virtual ~Session();
};

}  // namespace ultralight

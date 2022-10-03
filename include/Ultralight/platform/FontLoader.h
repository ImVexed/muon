///
/// @file FontLoader.h
///
/// @brief The header for the FontLoader interface.
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
#include <Ultralight/Buffer.h>

namespace ultralight {

///
/// Represents a font file, either on-disk path or in-memory file contents.
///
class UExport FontFile : public RefCounted {
public:
  ///
  /// Create a font file from an on-disk file path.
  ///
  /// @note  The file path should already exist.
  ///
  static Ref<FontFile> Create(const String16& filepath);

  ///
  /// Create a font file from an in-memory buffer.
  ///
  static Ref<FontFile> Create(Ref<Buffer> buffer);

  ///
  /// Whether or not this font file was created from an in-memory buffer.
  ///
  virtual bool is_in_memory() const = 0;

  ///
  /// The file path (if any).
  ///
  virtual String16 filepath() const = 0;

  ///
  /// The in-memory buffer (if any).
  ///
  virtual RefPtr<Buffer> buffer() const = 0;

  ///
  /// Unique hash (if this is a filepath, only the path string is hashed).
  ///
  virtual uint32_t hash() const = 0;

protected:
  FontFile();
  virtual ~FontFile();
  FontFile(const FontFile&);
  void operator=(const FontFile&);
};

///
/// @brief  Font Loader interface, used for all font lookup operations.
///
/// Every operating system has its own library of installed system fonts. The
/// FontLoader interface is used to lookup these fonts and fetch the actual
/// font data (raw TTF/OTF file data) for a given a certain font description.
///
/// AppCore automatically provides a platform-specific implementation of this
/// that loads installed fonts from the OS when you call App::Create().
///
/// If you are using Renderer::Create() instead, you will need to provide your
/// own implementation via `Platform::instance().set_font_loader(). For
/// convenience, you can still use AppCore's font loader implementation--
/// see the helper functions defined in <AppCore/Platform.h>.
///
/// To provide your own custom FontLoader implementation, you should inherit
/// from this class, handle the virtual member functions, and then pass an
/// instance of your class to `Platform::instance().set_font_loader()` before
/// calling Renderer::Create() or App::Create().
///
class UExport FontLoader {
public:
  virtual ~FontLoader();

  ///
  /// Fallback font family name. Will be used if all other fonts fail to load.
  ///
  /// @note  This font should be guaranteed to exist (eg, FontLoader::Load
  ///        won't fail when passed this font family name).
  ///
  virtual String16 fallback_font() const = 0;

  ///
  /// Fallback font family name that can render the specified characters. This
  /// is mainly used to support CJK (Chinese, Japanese, Korean) text display.
  ///
  /// @param  characters  One or more UTF-16 characters. This is almost always
  ///                     a single character.
  ///
  /// @param  weight      Font weight.
  ///
  /// @param  italic      Whether or not italic is requested.
  ///
  /// @return  Should return a font family name that can render the text.
  ///
  virtual String16 fallback_font_for_characters(const String16& characters,
                                                int weight,
                                                bool italic) const = 0;

  ///
  /// Get the actual font file data (TTF/OTF) for a given font description.
  /// 
  /// @param  family  Font family name.
  ///
  /// @param  weight  Font weight. 
  ///
  /// @param  italic  Whether or not italic is requested.
  ///
  /// @return  A font file matching the given description (either an on-disk
  ///          font filepath or an in-memory file contents). You can return
  ///          NULL here and the loader will fallback to another font.
  ///
  virtual RefPtr<FontFile> Load(const String16& family,
                                int weight,
                                bool italic) = 0;
};

}  // namespace ultralight

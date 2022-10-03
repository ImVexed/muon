///
/// @file Surface.h
///
/// @brief The header for the Surface and SurfaceFactory interfaces.
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
#include <Ultralight/Bitmap.h>
#include <Ultralight/Geometry.h>

namespace ultralight {

///
/// Offscreen pixel buffer surface. (Premultiplied BGRA 32-bit format)
///
/// When using the CPU renderer, each View is painted to its own Surface.
///
/// You can provide your own Surface implementation to make the renderer
/// paint directly to a block of memory controlled by you (this is useful for
/// lower-latency uploads to GPU memory or other platform-specific bitmaps).
///
/// A default Surface implementation, BitmapSurface, is automatically
/// provided by the library when you call Renderer::Create() without defining
/// a custom SurfaceFactory.
///
/// To provide your own custom Surface implementation, you should inherit
/// from this class, handle the virtual member functions, and then define a
/// custom SurfaceFactory that creates/destroys an instance of your class.
/// After that, you should pass an instance of your custom SurfaceFactory class
/// to `Platform::instance().set_font_loader()` before calling App::Create()
/// or Renderer::Create().
///
class UExport Surface {
public:
  virtual ~Surface();

  ///
  /// Width (in pixels).
  ///
  virtual uint32_t width() const = 0;

  ///
  /// Height (in pixels).
  ///
  virtual uint32_t height() const = 0;

  ///
  /// Number of bytes between rows (usually width * 4)
  ///
  virtual uint32_t row_bytes() const = 0;

  ///
  /// Size in bytes.
  ///
  virtual size_t size() const = 0;

  ///
  /// Lock the pixel buffer and get a pointer to the beginning of the data
  /// for reading/writing.
  ///
  /// Native pixel format is premultiplied BGRA 32-bit (8 bits per channel).
  ///
  virtual void* LockPixels() = 0;

  ///
  /// Unlock the pixel buffer.
  ///
  virtual void UnlockPixels() = 0;

  ///
  /// Resize the pixel buffer to a certain width and height (both in pixels).
  ///
  /// This should never be called while pixels are locked. 
  ///
  virtual void Resize(uint32_t width, uint32_t height) = 0;

  ///
  /// Set the dirty bounds to a certain value.
  ///
  /// This is called after the Renderer paints to an area of the pixel buffer.
  /// (The new value will be joined with the existing dirty_bounds())
  ///
  virtual void set_dirty_bounds(const IntRect& bounds);

  ///
  /// Get the dirty bounds.
  ///
  /// This value can be used to determine which portion of the pixel buffer has
  /// been updated since the last call to ClearDirtyBounds().
  ///
  /// The general algorithm to determine if a Surface needs display is:
  /// <pre>
  ///   if (!surface.dirty_bounds().IsEmpty()) {
  ///       // Surface pixels are dirty and needs display.
  ///       // Cast Surface to native Surface and use it here (pseudo code)
  ///       DisplaySurface(surface);
  ///
  ///       // Once you're done, clear the dirty bounds:
  ///       surface.ClearDirtyBounds();
  ///  }
  ///  </pre>
  ///
  virtual IntRect dirty_bounds() const;

  ///
  /// Clear the dirty bounds.
  ///
  /// You should call this after you're done displaying the Surface.
  ///
  virtual void ClearDirtyBounds();

protected:
  Surface();

  IntRect dirty_bounds_;
};

///
/// SurfaceFactory can be used to provide your own native Surface implementation.
///
/// This can be used to wrap a platform-specific GPU texture, Windows DIB,
/// macOS CGImage, or any other pixel buffer target for display on screen.
///
/// The default factory creates/destroys a BitmapSurface but you can override
/// this by providing your own factory to Platform::set_surface_factory.
///
class UExport SurfaceFactory {
public:
  virtual ~SurfaceFactory();

  ///
  /// Create a native Surface with a certain width and height (in pixels).
  ///
  virtual Surface* CreateSurface(uint32_t width, uint32_t height) = 0;

  ///
  /// Destroy a native Surface previously created by CreateSurface().
  ///
  virtual void DestroySurface(Surface* surface) = 0;
};

///
/// The default Surface implementation, backed by a Bitmap.
///
class UExport BitmapSurface : public Surface {
public:
  virtual uint32_t width() const override;

  virtual uint32_t height() const override;

  virtual uint32_t row_bytes() const override;

  virtual size_t size() const override;

  virtual void* LockPixels() override;

  virtual void UnlockPixels() override;

  virtual void Resize(uint32_t width, uint32_t height) override;

  ///
  /// Get the underlying Bitmap.
  ///
  RefPtr<Bitmap> bitmap();

protected:
  BitmapSurface(uint32_t width, uint32_t height);
  virtual ~BitmapSurface();
  BitmapSurface(const BitmapSurface&) = delete;
  void operator=(const BitmapSurface&) = delete;
  friend class BitmapSurfaceFactory;

  void* impl_;
};

///
/// Get the default Bitmap Surface Factory singleton. (Do not destroy this,
/// this singleton is owned by the library).
///
UExport SurfaceFactory* GetBitmapSurfaceFactory();

}  // namespace ultralight

///
/// @file Config.h
///
/// @brief The header for the Config struct.
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
/// The winding order for front-facing triangles. (This is only used when the
/// GPU renderer is enabled)
///
/// @note  In most 3D engines, there is the concept that triangles have a
///        a "front" and a "back". All the front-facing triangles (eg, those
///        that are facing the camera) are rendered, and all back-facing
///        triangles are culled (ignored). The winding-order of the triangle's
///        vertices is used to determine which side is front and back. You
///        should tell Ultralight which winding-order your 3D engine uses.
///
enum FaceWinding {
  ///
  /// Clockwise Winding (Direct3D, etc.)
  ///
  kFaceWinding_Clockwise,

  ///
  /// Counter-Clockwise Winding (OpenGL, etc.)
  ///
  kFaceWinding_CounterClockwise,
};

enum FontHinting {
  ///
  /// Lighter hinting algorithm-- glyphs are slightly fuzzier but better
  /// resemble their original shape. This is achieved by snapping glyphs to the
  /// pixel grid only vertically which better preserves inter-glyph spacing.
  ///
  kFontHinting_Smooth,

  ///
  /// Default hinting algorithm-- offers a good balance between sharpness and
  /// shape at smaller font sizes.
  ///
  kFontHinting_Normal,

  ///
  /// Strongest hinting algorithm-- outputs only black/white glyphs. The result
  /// is usually unpleasant if the underlying TTF does not contain hints for
  /// this type of rendering.
  ///
  kFontHinting_Monochrome,
};

///
/// @brief  Configuration settings for Ultralight.         
///
/// This is intended to be implemented by users and defined before creating the
/// Renderer. @see Platform::set_config.
///
struct UExport Config {
  ///
  /// The file path to the directory that contains Ultralight's bundled
  /// resources (eg, cacert.pem and other localized resources). 
  ///
  String16 resource_path;

  ///
  /// The file path to a writable directory that will be used to store cookies,
  /// cached resources, and other persistent data.
  ///
  String16 cache_path;

  ///
  /// When enabled, each View will be rendered to an offscreen GPU texture
  /// using the GPU driver set in Platform::set_gpu_driver. You can fetch
  /// details for the texture via View::render_target.
  ///
  /// When disabled (the default), each View will be rendered to an offscreen
  /// pixel buffer. This pixel buffer can optionally be provided by the user--
  /// for more info see <Ultralight/platform/Surface.h> and View::surface.
  ///
  bool use_gpu_renderer = false;

  ///
  /// The amount that the application DPI has been scaled (200% = 2.0).
  /// This should match the device scale set for the current monitor.
  ///
  /// Note: Device scales are rounded to nearest 1/8th (eg, 0.125).
  ///
  double device_scale = 1.0;

  ///
  /// The winding order for front-facing triangles. @see FaceWinding
  ///
  /// Note: This is only used when the GPU renderer is enabled.
  ///
  FaceWinding face_winding = kFaceWinding_CounterClockwise;

  ///
  /// Whether or not images should be enabled.
  ///
  bool enable_images = true;

  ///
  /// Whether or not JavaScript should be enabled.
  ///
  bool enable_javascript = true;

  ///
  /// The hinting algorithm to use when rendering fonts. @see FontHinting
  ///
  FontHinting font_hinting = kFontHinting_Normal;

  ///
  /// The gamma to use when compositing font glyphs, change this value to
  /// adjust contrast (Adobe and Apple prefer 1.8, others may prefer 2.2).
  ///
  double font_gamma = 1.8;

  ///
  /// Default font-family to use.
  ///
  String16 font_family_standard = "Times New Roman";

  ///
  /// Default font-family to use for fixed fonts. (pre/code)
  ///
  String16 font_family_fixed = "Courier New";

  ///
  /// Default font-family to use for serif fonts.
  ///
  String16 font_family_serif = "Times New Roman";

  ///
  /// Default font-family to use for sans-serif fonts.
  ///
  String16 font_family_sans_serif = "Arial";

  ///
  /// Default user-agent string.
  ///
  String16 user_agent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                          "AppleWebKit/608.3.10 (KHTML, like Gecko) "
                          "Ultralight/1.2.0 Safari/608.3.10";

  ///
  /// Default user stylesheet. You should set this to your own custom CSS
  /// string to define default styles for various DOM elements, scrollbars,
  /// and platform input widgets.
  ///
  String16 user_stylesheet;

  ///
  /// Whether or not we should continuously repaint any Views or compositor
  /// layers, regardless if they are dirty or not. This is mainly used to
  /// diagnose painting/shader issues.
  ///
  bool force_repaint = false;

  ///
  /// When a CSS animation is active, the amount of time (in seconds) to wait
  /// before triggering another repaint. Default is 60 Hz.
  ///
  double animation_timer_delay = 1.0 / 60.0;

  ///
  /// When a smooth scroll animation is active, the amount of time (in seconds)
  /// to wait before triggering another repaint. Default is 60 Hz.
  ///
  double scroll_timer_delay = 1.0 / 60.0;

  ///
  /// The amount of time (in seconds) to wait before running the recycler (will
  /// attempt to return excess memory back to the system).
  ///
  double recycle_delay = 4.0;

  ///
  /// Size of WebCore's memory cache in bytes. 
  ///
  /// @note  You should increase this if you anticipate handling pages with
  ///        large resources, Safari typically uses 128+ MiB for its cache.
  ///
  uint32_t memory_cache_size = 64 * 1024 * 1024;

  ///
  /// Number of pages to keep in the cache. Defaults to 0 (none).
  ///
  /// @note  Safari typically caches about 5 pages and maintains an on-disk
  ///        cache to support typical web-browsing activities. If you increase
  ///        this, you should probably increase the memory cache size as well.
  ///
  uint32_t page_cache_size = 0;

  ///
  /// JavaScriptCore tries to detect the system's physical RAM size to set
  /// reasonable allocation limits. Set this to anything other than 0 to
  /// override the detected value. Size is in bytes.
  ///
  /// This can be used to force JavaScriptCore to be more conservative with
  /// its allocation strategy (at the cost of some performance).
  ///
  uint32_t override_ram_size = 0;

  ///
  /// The minimum size of large VM heaps in JavaScriptCore. Set this to a
  /// lower value to make these heaps start with a smaller initial value.
  ///
  uint32_t min_large_heap_size = 32 * 1024 * 1024;

  ///
  /// The minimum size of small VM heaps in JavaScriptCore. Set this to a
  /// lower value to make these heaps start with a smaller initial value.
  ///
  uint32_t min_small_heap_size = 1 * 1024 * 1024;
};

}  // namespace ultralight

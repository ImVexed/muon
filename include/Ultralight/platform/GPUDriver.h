///
/// @file GPUDriver.h
///
/// @brief The header for the GPUDriver interface.
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
#pragma warning(disable: 4251)
#include <Ultralight/Defines.h>
#include <Ultralight/Geometry.h>
#include <Ultralight/Matrix.h>
#include <Ultralight/Bitmap.h>

namespace ultralight {

///
/// @note  This pragma pack(push, 1) command is important! Vertex layouts
///	       should not be padded with any bytes.
///
#pragma pack(push, 1)

///
/// RenderBuffer description, @see GPUDriver::CreateRenderBuffer.
///
struct UExport RenderBuffer {
  uint32_t texture_id;      // The backing texture for this RenderBuffer
  uint32_t width;           // The width of the RenderBuffer texture
  uint32_t height;          // The height of the RenderBuffer texture
  bool has_stencil_buffer;  // Currently unused, always false.
  bool has_depth_buffer;    // Currently unsued, always false.
};

///
/// Vertex layout for path vertices, useful for synthesizing or modifying
/// vertex data.
///
struct Vertex_2f_4ub_2f {
  float pos[2];
  unsigned char color[4];
  float obj[2];
};

///
/// Vertex layout for quad vertices, useful for synthesizing or modifying
/// vertex data.
///
struct Vertex_2f_4ub_2f_2f_28f {
  float pos[2];
  unsigned char color[4];
  float tex[2];
  float obj[2];
  float data0[4];
  float data1[4];
  float data2[4];
  float data3[4];
  float data4[4];
  float data5[4];
  float data6[4];
};

///
/// Vertex formats
///
enum UExport VertexBufferFormat {
  kVertexBufferFormat_2f_4ub_2f,
  kVertexBufferFormat_2f_4ub_2f_2f_28f,
};

///
/// Vertex buffer, @see GPUDriver::CreateGeometry
///
struct UExport VertexBuffer {
  VertexBufferFormat format;
  uint32_t size;
  uint8_t* data;
};

///
/// Vertex index type
///
typedef uint32_t IndexType;

///
/// Vertex index buffer, @see GPUDriver::CreateGeometry
///
struct UExport IndexBuffer {
  uint32_t size;
  uint8_t* data;
};

///
/// Shader types, used by GPUState::shader_type
///
/// Each of these correspond to a vertex/pixel shader pair. You can find
/// stock shader code for these in the `shaders` folder of the AppCore repo.
///
enum UExport ShaderType {
  kShaderType_Fill,      // Shader program for quad geometry
  kShaderType_FillPath,  // Shader program for path geometry
};

///
/// GPU state description.
///
struct UExport GPUState {
  /// Viewport width in pixels
  uint32_t viewport_width;
  
  /// Viewport height in pixels
  uint32_t viewport_height;
  
  /// Transform matrix-- you should multiply this with the screen-space
  /// orthographic projection matrix then pass to the vertex shader.
  Matrix4x4 transform;
  
  /// Whether or not we should enable texturing for the current draw command.
  bool enable_texturing;
  
  /// Whether or not we should enable blending for the current draw command.
  /// If blending is disabled, any drawn pixels should overwrite existing.
  /// Mainly used so we can modify alpha values of the RenderBuffer during
  /// scissored clears.
  bool enable_blend;
  
  /// The vertex/pixel shader program pair to use for the current draw command.
  /// You should cast this to ShaderType to get the corresponding enum.
  uint8_t shader_type;
  
  /// The render buffer to use for the current draw command.
  uint32_t render_buffer_id;
  
  /// The texture id to bind to slot #1. (Will be 0 if none)
  uint32_t texture_1_id;
  
  /// The texture id to bind to slot #2. (Will be 0 if none)
  uint32_t texture_2_id;
  
  /// The texture id to bind to slot #3. (Will be 0 if none)
  uint32_t texture_3_id;
  
  /// The following four members are passed to the pixel shader via uniforms.
  float uniform_scalar[8];
  vec4 uniform_vector[8];
  uint8_t clip_size;
  Matrix4x4 clip[8];
  
  /// Whether or not scissor testing should be used for the current draw command.
  bool enable_scissor;
  
  /// The scissor rect to use for scissor testing (units in pixels)
  IntRect scissor_rect;
};

///
/// Command types, used by Command::command_type
///
enum UExport CommandType {
  kCommandType_ClearRenderBuffer,
  kCommandType_DrawGeometry,
};

///
/// Command description.
///
struct UExport Command {
  uint8_t command_type;    // The type of command to dispatch.
  GPUState gpu_state;      // GPU state parameters for current command.

  /// The following members are only used with kCommandType_DrawGeometry
  uint32_t geometry_id;    // The geometry ID to bind
  uint32_t indices_count;  // The number of indices
  uint32_t indices_offset; // The index to start from
};

///
/// Command list, @see GPUDriver::UpdateCommandList
///
struct UExport CommandList {
  uint32_t size;
  Command* commands;
};

#pragma pack(pop)

///
/// @brief  GPUDriver interface, dispatches GPU calls to the native driver.
///
/// This is automatically provided for you when you use App::Create(), AppCore
/// provides platform-specific implementations of GPUDriver for each OS.
///
/// If you are using Renderer::Create(), you will need to provide your own
/// implementation of this class if you have enabled the GPU renderer in the
/// Config. @see Platform::set_gpu_driver
///
class UExport GPUDriver {
public:
  virtual ~GPUDriver();

  ///
  /// Called before any commands are dispatched during a frame.
  ///
  virtual void BeginSynchronize() = 0;

  ///
  /// Called after any commands are dispatched during a frame.
  ///
  virtual void EndSynchronize() = 0;

  ///
  /// Get the next available texture ID.
  ///
  virtual uint32_t NextTextureId() = 0;

  ///
  /// Create a texture with a certain ID and optional bitmap.
  ///
  /// **NOTE**: If the Bitmap is empty (Bitmap::IsEmpty), then a RTT Texture
  ///           should be created instead. This will be used as a backing
  ///           texture for a new RenderBuffer.
  ///
  virtual void CreateTexture(uint32_t texture_id,
                             Ref<Bitmap> bitmap) = 0;

  ///
  /// Update an existing non-RTT texture with new bitmap data.
  ///
  virtual void UpdateTexture(uint32_t texture_id,
                             Ref<Bitmap> bitmap) = 0;

  ///
  /// Destroy a texture.
  ///
  virtual void DestroyTexture(uint32_t texture_id) = 0;

  ///
  /// Generate the next available render buffer ID.
  ///
  virtual uint32_t NextRenderBufferId() = 0;

  ///
  /// Create a render buffer with certain ID and buffer description.
  ///
  virtual void CreateRenderBuffer(uint32_t render_buffer_id,
                                  const RenderBuffer& buffer) = 0;

  ///
  /// Destroy a render buffer
  ///
  virtual void DestroyRenderBuffer(uint32_t render_buffer_id) = 0;

  ///
  /// Generate the next available geometry ID.
  ///
  virtual uint32_t NextGeometryId() = 0;

  ///
  /// Create geometry with certain ID and vertex/index data.
  ///
  virtual void CreateGeometry(uint32_t geometry_id,
                              const VertexBuffer& vertices,
                              const IndexBuffer& indices) = 0;

  ///
  /// Update existing geometry with new vertex/index data.
  ///
  virtual void UpdateGeometry(uint32_t geometry_id,
                              const VertexBuffer& vertices,
                              const IndexBuffer& indices) = 0;

  ///
  /// Destroy geometry.
  ///
  virtual void DestroyGeometry(uint32_t geometry_id) = 0;

  ///
  /// Update command list (you should copy the commands to your own structure).
  ///
  virtual void UpdateCommandList(const CommandList& list) = 0;
};

}  // namespace ultralight

///
/// @file Matrix.h
///
/// @brief The header for Matrix helpers
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
#include <Ultralight/Geometry.h>

namespace ultralight {

///
/// 4x4 Matrix Helper
///
struct UExport Matrix4x4 {
  ///
  /// Raw 4x4 matrix as an array
  ///
  float data[16];

  ///
  /// Set to identity matrix.
  ///
  void SetIdentity();
};

///
/// Transformation Matrix helper
///
struct UExport Matrix {
#if defined(__x86_64__) || defined(_M_X64)
#if defined(_MSC_VER)
  __declspec(align(16)) typedef double Aligned4x4[4][4];
#else
  typedef double Aligned4x4[4][4] __attribute__((aligned(16)));
#endif
#else
  typedef double Aligned4x4[4][4];
#endif

  Aligned4x4 data;

  ///
  /// Set to identity matrix.
  ///
  void SetIdentity();

  ///
  /// Set to an orthographic projection matrix suitable for use with our
  /// vertex shaders. Optionally flip the y-coordinate space (eg, for OpenGL).
  ///
  void SetOrthographicProjection(double screen_width, double screen_height,
    bool flip_y);

  ///
  /// Set to another matrix.
  ///
  void Set(const Matrix& other);

  ///
  /// Set to another matrix.
  ///
  void Set(const Matrix4x4& other);
  
  ///
  /// Set from raw affine members.
  ///
  void Set(double a, double b, double c, double d, double e, double f);

  ///
  /// Set from raw 4x4 components.
  ///
  void Set(double m11, double m12, double m13, double m14,
           double m21, double m22, double m23, double m24,
           double m31, double m32, double m33, double m34,
           double m41, double m42, double m43, double m44);

  inline double m11() const { return data[0][0]; }
  inline double m12() const { return data[0][1]; }
  inline double m13() const { return data[0][2]; }
  inline double m14() const { return data[0][3]; }
  inline double m21() const { return data[1][0]; }
  inline double m22() const { return data[1][1]; }
  inline double m23() const { return data[1][2]; }
  inline double m24() const { return data[1][3]; }
  inline double m31() const { return data[2][0]; }
  inline double m32() const { return data[2][1]; }
  inline double m33() const { return data[2][2]; }
  inline double m34() const { return data[2][3]; }
  inline double m41() const { return data[3][0]; }
  inline double m42() const { return data[3][1]; }
  inline double m43() const { return data[3][2]; }
  inline double m44() const { return data[3][3]; }

  inline double a() const { return data[0][0]; }
  inline double b() const { return data[0][1]; }
  inline double c() const { return data[1][0]; }
  inline double d() const { return data[1][1]; }
  inline double e() const { return data[3][0]; }
  inline double f() const { return data[3][1]; }

  ///
  /// Whether or not this is an identity matrix.
  ///
  bool IsIdentity() const;

  ///
  /// Whether or not this is an identity matrix or translation.
  ///
  bool IsIdentityOrTranslation() const;

  ///
  /// Whether or not this matrix uses only affine transformations.
  ///
  bool IsAffine() const;

  ///
  /// Whether or not this is an identity, translation, or non-negative
  /// uniform scale.
  ///
  bool IsSimple() const;

  ///
  /// Translate by x and y.
  ///
  void Translate(double x, double y);

  ///
  /// Scale by x and y.
  ///
  void Scale(double x, double y);

  /// 
  /// Rotate matrix by theta (in degrees)
  ///
  void Rotate(double theta);

  ///
  /// Rotate matrix by x and y
  ///
  void Rotate(double x, double y);

  ///
  /// Transform (multiply) by another Matrix
  ///
  void Transform(const Matrix& other);

  ///
  /// Get the inverse of this matrix. May return false if not invertible.
  ///
  bool GetInverse(Matrix& result) const;

  ///
  /// Transform point by this matrix and get the result.
  ///
  Point Apply(const Point& p) const;

  ///
  /// Transform rect by this matrix and get the result as an axis-aligned rect.
  ///
  Rect Apply(const Rect& r) const;

  ///
  /// Get an integer hash of this matrix's members.
  ///
  uint32_t Hash() const;

  ///
  /// Get this matrix as unaligned 4x4 float components (for use passing to
  /// GPU driver APIs). 
  ///
  Matrix4x4 GetMatrix4x4() const;
};

bool UExport operator==(const Matrix& a, const Matrix& b);
bool UExport operator!=(const Matrix& a, const Matrix& b);

bool UExport operator==(const Matrix4x4& a, const Matrix4x4& b);
bool UExport operator!=(const Matrix4x4& a, const Matrix4x4& b);

}  // namespace ultralight

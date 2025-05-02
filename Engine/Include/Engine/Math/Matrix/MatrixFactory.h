#pragma once
#include <Engine/Math/Matrix/Matrix.h>
class MatrixFactory
{
public:
  // Creates a translation matrix for object movement.
  static Matrix CreateTranslation(float tx, float ty, float tz);

  // Creates a scaling matrix to change the size of an object.
  static Matrix CreateScaling(float sx, float sy, float sz);

  // Creates a rotation matrix around the X-axis.
  static Matrix CreateRotationX(float angleInRadians);

  // Creates a rotation matrix around the Y-axis.
  static Matrix CreateRotationY(float angleInRadians);

  // Creates a rotation matrix around the Z-axis.
  static Matrix CreateRotationZ(float angleInRadians);

  // Creates a look-at matrix for camera view.
  static Matrix CreateLookAt(const DirectX::FXMVECTOR& eye, const DirectX::FXMVECTOR& target, const DirectX::FXMVECTOR& up);

  // Creates a perspective matrix for projecting 3D scenes onto a 2D screen.
  static Matrix CreatePerspective(float fovY, float aspectRatio, float nearPlane, float farPlane);

  // Creates an orthographic projection matrix.
  static Matrix CreateOrthographic(float width, float height, float nearPlane, float farPlane);

  static Matrix CreateView(DirectX::FXMVECTOR& lookAt, DirectX::FXMVECTOR& upDir, DirectX::FXMVECTOR& cameraPos);
};

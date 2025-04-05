#include <Include/Engine/Math/Matrix/MatrixFactory.h>

Matrix MatrixFactory::CreateTranslation(float tx, float ty, float tz)
{
  return Matrix(DirectX::XMMatrixTranslation(tx, ty, tz));
}

Matrix MatrixFactory::CreateScaling(float sx, float sy, float sz)
{
  return Matrix(DirectX::XMMatrixScaling(sx, sy, sz));
}

Matrix MatrixFactory::CreateRotationX(float angleInRadians)
{
  return Matrix(DirectX::XMMatrixRotationX(angleInRadians));
}

Matrix MatrixFactory::CreateRotationY(float angleInRadians)
{
  return Matrix(DirectX::XMMatrixRotationY(angleInRadians));
}

Matrix MatrixFactory::CreateRotationZ(float angleInRadians)
{
  return Matrix(DirectX::XMMatrixRotationZ(angleInRadians));
}

Matrix MatrixFactory::CreateLookAt(const DirectX::FXMVECTOR& eye, const DirectX::FXMVECTOR& target, const DirectX::FXMVECTOR& up)
{
  return Matrix(DirectX::XMMatrixLookAtLH(eye, target, up));
}

Matrix MatrixFactory::CreatePerspective(float fovY, float aspectRatio, float nearPlane, float farPlane)
{
  return Matrix(DirectX::XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearPlane, farPlane));
}

Matrix MatrixFactory::CreateOrthographic(float width, float height, float nearPlane, float farPlane)
{
  return Matrix(DirectX::XMMatrixOrthographicLH(width, height, nearPlane, farPlane));
}

Matrix MatrixFactory::CreateView(DirectX::FXMVECTOR& lookAt, DirectX::FXMVECTOR& upDir, DirectX::FXMVECTOR& cameraPos)
{
  return Matrix(DirectX::XMMatrixLookAtLH(cameraPos, lookAt, upDir));
}
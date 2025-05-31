#pragma once
#include <Utils/WinInclude.hpp>
#include <DirectXMath.h>
class Matrix
{
public:
  // Default constructor, initializes the matrix to an identity matrix.
  Matrix();
  Matrix(const DirectX::XMMATRIX& other);
  Matrix(const Matrix& other);

  // Matrix multiplication (this * other)
  Matrix operator*(const Matrix& other) const;

  // Scalar multiplication (this * scalar)
  Matrix operator*(float scalar) const;

  // Matrix addition (this + other)
  Matrix operator+(const Matrix& other) const;

  // Matrix subtraction (this - other)
  Matrix operator-(const Matrix& other) const;

  // Accessor for modifying matrix rows.
  float* operator[](int row);

  // Const accessor for reading matrix rows.
  const float* operator[](int row) const;

  // Equality comparison between matrices.
  bool operator==(const Matrix& other) const;

  Matrix& operator=(const Matrix& other);
  Matrix& operator=(const DirectX::XMMATRIX& other);

private:
  DirectX::XMMATRIX m_matrix;  // Matrix data stored as an XMMATRIX
};

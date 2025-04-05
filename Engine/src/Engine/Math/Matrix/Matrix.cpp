#include <Include/Engine/Math/Matrix/Matrix.h>

Matrix::Matrix()
{
  m_matrix = DirectX::XMMatrixIdentity();
}

Matrix::Matrix(const Matrix& other) {
  m_matrix = other.m_matrix;
}

Matrix::Matrix(const DirectX::XMMATRIX& other) {
  m_matrix = other;
}

Matrix Matrix::operator*(const Matrix& other) const
{
  return Matrix(this->m_matrix * other.m_matrix);
}

Matrix Matrix::operator*(float scalar) const
{
  return Matrix(this->m_matrix * scalar);
}

Matrix Matrix::operator+(const Matrix& other) const
{
  return Matrix(this->m_matrix + other.m_matrix);
}

Matrix Matrix::operator-(const Matrix& other) const
{
  return Matrix(this->m_matrix - other.m_matrix);
}

float* Matrix::operator[](int row)
{
  return m_matrix.r[row].m128_f32;
}

const float* Matrix::operator[](int row) const
{
  return m_matrix.r[row].m128_f32;
}

bool Matrix::operator==(const Matrix& other) const
{
  for (size_t i = 0; i < 4; i++)
  {
    for (size_t j = 0; j < 4; j++)
    {
      if (m_matrix.r[i].m128_f32[j] != other.m_matrix.r[i].m128_f32[j])
      {
        return false;
      }
    }
  }
  return true;
}

Matrix& Matrix::operator=(const Matrix& other)
{
  this->m_matrix = other.m_matrix;
  return *this;
}

Matrix& Matrix::operator=(const DirectX::XMMATRIX& other)
{
  this->m_matrix = other;
  return *this;
}


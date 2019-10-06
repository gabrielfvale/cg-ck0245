#include "Matrix4.hpp"

Matrix4::Matrix4()
{
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      matrix_[i][j] = 0;
}

void Matrix4::set(int i, int j, float value) { matrix_[i][j] = value; }
float Matrix4::get(int i, int j) { return matrix_[i][j]; }

Matrix4 Matrix4::operator+(Matrix4& m)
{
	Matrix4 ret;
	
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      ret.set(i, j, matrix_[i][j] + m.get(i, j));
	return ret;
}

Matrix4 Matrix4::operator-(Matrix4& m)
{
	Matrix4 ret;
	
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 4; j++)
      ret.set(i, j, matrix_[i][j] - m.get(i, j));
	return ret;
}

Point Matrix4::operator*(Point& p)
{
  float ox, oy, oz;
  float newx, newy, newz;
  p.get_coordinates(&ox, &oy, &oz);
  newx = matrix_[0][0] * ox + matrix_[0][1] * oy + matrix_[0][2] * oz + matrix_[0][3];
  newy = matrix_[1][0] * ox + matrix_[1][1] * oy + matrix_[1][2] * oz + matrix_[1][3];
  newz = matrix_[2][0] * ox + matrix_[2][1] * oy + matrix_[2][2] * oz + matrix_[2][3];

  return Point(newx, newy, newz);
}

Vector3 Matrix4::operator*(Vector3& v)
{
  float ox, oy, oz;
  float newx, newy, newz;
  v.get_coordinates(&ox, &oy, &oz);
  newx = matrix_[0][0] * ox + matrix_[0][1] * oy + matrix_[0][2] * oz;
  newy = matrix_[1][0] * ox + matrix_[1][1] * oy + matrix_[1][2] * oz;
  newz = matrix_[2][0] * ox + matrix_[2][1] * oy + matrix_[2][2] * oz;

  return Vector3(newx, newy, newz);
}

Quaternion Matrix4::operator*(Quaternion& q)
{
  float ox, oy, oz;
  float newx, newy, newz;
  Vector3* vec = q.get_vec();
  (*vec).get_coordinates(&ox, &oy, &oz);
  newx = matrix_[0][0] * ox + matrix_[0][1] * oy + matrix_[0][2] * oz + matrix_[0][3] * q.get_scalar();
  newy = matrix_[1][0] * ox + matrix_[1][1] * oy + matrix_[1][2] * oz + matrix_[1][3] * q.get_scalar();
  newz = matrix_[2][0] * ox + matrix_[2][1] * oy + matrix_[2][2] * oz + matrix_[2][3] * q.get_scalar();

  return Quaternion(Vector3(newx, newy, newz), q.get_scalar());
}

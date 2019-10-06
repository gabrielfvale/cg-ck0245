#include <cmath>

#include "Quaternion.hpp"
#include "Matrix4.hpp"

Quaternion::Quaternion()
{
  v_ = Vector3();
  w_ = 1;
}
Quaternion::Quaternion(Vector3 v, float w)
{
  v_ = v;
  w_ = w;
}
Quaternion::Quaternion(float x, float y, float z, float w)
{
  v_ = Vector3(x, y, z);
  w_ = w;
}

Vector3* Quaternion::get_vec() { return &v_; }
float Quaternion::get_scalar() { return w_; }

void Quaternion::set_vec(Vector3& v) { v_ = v; }
void Quaternion::set_scalar(float w) { w_ = w; }
void Quaternion::set(float x, float y, float z, float w) { v_ = Vector3(x, y, z); w_ = w; }

float Quaternion::length()
{
  return std::sqrt(std::pow(v_.norm(), 2) + w_*w_);
}

Quaternion Quaternion::conjugate() { return Quaternion(v_ * (-1), w_); }

Quaternion Quaternion::operator*(Quaternion& qr)
{
  Matrix4 tr_matrix = Matrix4();
  float vlx, vly, vlz;
  v_.get_coordinates(&vlx, &vly, &vlz);

  tr_matrix.set(0, 0, w_);
  tr_matrix.set(0, 1, -vlz);
  tr_matrix.set(0, 2, vly);
  tr_matrix.set(0, 3, vlx);

  tr_matrix.set(1, 0, vlz);
  tr_matrix.set(1, 1, w_);
  tr_matrix.set(1, 2, -vlx);
  tr_matrix.set(1, 3, vly);

  tr_matrix.set(2, 0, -vly);
  tr_matrix.set(2, 1, vlx);
  tr_matrix.set(2, 2, w_);
  tr_matrix.set(2, 3, vlz);

  tr_matrix.set(3, 0, -vlx);
  tr_matrix.set(3, 1, -vly);
  tr_matrix.set(3, 2, -vlz);
  tr_matrix.set(3, 3, w_);

  return tr_matrix * qr;
}

#include <cmath>
#include <string>

#include "Vector3.h"
#include "Point.h"

Vector3::Vector3(){ x_ = 0; y_ = 0; z_ = 0; }

Vector3::Vector3(float x, float y, float z)
{
  origin_ = Point();
  x_ = x; y_ = y; z_ = z;
}
Vector3::Vector3(Point* o, float x, float y, float z)
{
  origin_ = *o;
  x_ = x; y_ = y; z_ = z;
}
Vector3::Vector3(Point* start, Point* end)
{
  origin_ = *start;
  x_ = (*end).get_x() - (*start).get_x();
  y_ = (*end).get_y() - (*start).get_y();
  z_ = (*end).get_z() - (*start).get_z();
};

float Vector3::get_x() { return x_; }
float Vector3::get_y() { return y_; }
float Vector3::get_z() { return z_; }

void Vector3::get_coordinates(float* x, float* y, float* z)
{
  *x = x_;
  *y = y_;
  *z = z_;
}

void Vector3::set_coordinates(float x, float y, float z)
{
  x_ = x;
  y_ = y;
  z_ = z;
}

float Vector3::norm() { return std::sqrt(dot_product(this)); }

void Vector3::normalize()
{
  float norm = this->norm();
  x_ /= norm;
  y_ /= norm;
  z_ /= norm;
}

float Vector3::dot_product(Vector3* v)
{
  float v1, v2, v3;
  (*v).get_coordinates(&v1, &v2, &v3);
  return x_*v1 + y_*v2 + z_*v3;
}

Vector3 Vector3::cross_product(Vector3* v)
{
  // Creates an orthogonal vector to 'this' and v;
  // u x v = (u2.v3 − u3.v2, u3.v1 − u1.v3, u1.v2 − u2.v1)
  // u x v != v x u
  float v1, v2, v3;
  (*v).get_coordinates(&v1, &v2, &v3);
  return Vector3(y_*v3 - z_*v2, z_*v1 - x_*v3, x_*v2 - y_*v1);
}

bool Vector3::is_orthogonal(Vector3* v)
{
  return dot_product(v) == 0 ? true : false;
}

std::string Vector3::to_string()
{
  return std::string() + "(" + std::to_string(x_) + ", " + std::to_string(y_) + ", " + std::to_string(z_) + ")";
}

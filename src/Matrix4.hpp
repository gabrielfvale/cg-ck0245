#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "Point.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

class Matrix4
{
  private:
    float matrix_[4][4];
  public:
    Matrix4();
    Matrix4 operator+(Matrix4& m);
    Matrix4 operator-(Matrix4& m);
    float& operator()(int i, int j);
    Point operator*(Point& p);
    Vector3 operator*(Vector3& v);
    Quaternion operator*(Quaternion& q);
};

#endif

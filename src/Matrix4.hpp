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
    void set(int i, int j, float value);
    float get(int i, int j);
    Matrix4 operator+(Matrix4& m);
    Matrix4 operator-(Matrix4& m);
    Point operator*(Point& p);
    Vector3 operator*(Vector3& v);
    Quaternion operator*(Quaternion& q);
};

#endif

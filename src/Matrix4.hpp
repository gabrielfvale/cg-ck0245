#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "Point.hpp"
#include "Vector3.hpp"

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
    Vector3 operator*(Vector3& v);
    Point operator*(Point& p);
};

#endif

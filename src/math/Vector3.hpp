#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <string>
#include "Point.hpp"

class Vector3
{
  private:
    Point origin_;
    float x_, y_, z_;
  public:
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(Point* o, float x, float y, float z);
    Vector3(float* coords);
    Vector3(Point* start, Point* end);
    float get_x();
    float get_y();
    float get_z();
    void get_coordinates(float* x, float* y, float* z);
    void set_coordinates(float x,  float y, float z);
    void to_float(float* cpy);
    Vector3 operator+(Vector3& v);
    Vector3 operator-(Vector3& v);
    Vector3 operator*(float value);
    Vector3 operator*(Vector3& v);
    Vector3 operator/(float value);
    Vector3& operator+=(Vector3& v);
    Vector3& operator-=(Vector3& v);
    Vector3& operator*=(Vector3& v);
    Vector3& operator/=(Vector3& v);
    float norm();
    void normalize();
    float dot_product(Vector3* v);
    Vector3 cross_product(Vector3* v);
    bool is_orthogonal(Vector3* v);
    friend std::ostream& operator<<(std::ostream& stream, Vector3& vector);
};

#endif

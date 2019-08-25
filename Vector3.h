#ifndef VECTOR3_H
#define VECTOR3_H

#include "Point.h"
#include <string>

class Vector3
{
  private:
    Point origin_;
    float x_, y_, z_;
  public:
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(Point* o, float x, float y, float z);
    Vector3(Point* start, Point* end);
    float get_x();
    float get_y();
    float get_z();
    void get_coordinates(float* x, float* y, float* z);
    void set_coordinates(float x,  float y, float z);
    void add(Vector3* v);
    void subtract(Vector3* v);
    void multiply(float s);
    float norm();
    void normalize();
    float dot_product(Vector3* v);
    Vector3 cross_product(Vector3* v);
    bool is_orthogonal(Vector3* v);
    std::string to_string();
};

#endif

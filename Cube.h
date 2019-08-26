#ifndef CUBE_H
#define CUBE_H

#include "Point.h"
#include "Vector3.h"

class Cube
{
  private:
    Point center_; // center of the cube
    Vector3 u_, v_, w_; // orthonormal cube axis
    float edge_; // edge length
  public:
    Cube();
    Cube(Point center, Vector3 u, Vector3 v, float edge);
    Point* get_center();
    void get_axis(Vector3* u, Vector3* v, Vector3* w);
    float* get_edge();
    void set_params(Point* center, float* edge);
};

#endif

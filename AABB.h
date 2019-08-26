#ifndef AABB_H
#define AABB_H

#include "Point.h"
#include "Vector3.h"

class AABB
{
  private:
    Point center_; // center of the cube
    Vector3 n_; // AABB axis (unit vector)
    float edge_; // edge length
  public:
    AABB();
    AABB(Point center, Vector3 n, float edge);
    Point* get_center();
    Vector3* get_axis();
    float* get_edge();
    void set_params(Point* center, Vector3* n, float* edge);
};

#endif

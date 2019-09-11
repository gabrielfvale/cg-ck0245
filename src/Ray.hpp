#ifndef RAY_HPP
#define RAY_HPP

#include <vector>
#include "Point.hpp"
#include "Vector3.hpp"

class Ray
{
  private:
    Point p0_; // known point of the ray
    Vector3 d_; // unit vector that determines the ray's direction
  public:
    Ray();
    Ray(Point p0, Vector3 d);
    Point calc_point(float t);
    Point get_p0();
    Vector3 get_d();
    void set_params(Point* p0, Vector3* d);
};

#endif

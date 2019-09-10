#ifndef PLANE_HPP
#define PLANE_HPP

#include "Object.hpp"
#include "Point.hpp"
#include "Vector3.hpp"

class Plane : public Object
{
  private:
    Point p0_; // known point of the plane
    Vector3 n_; // unit vector orthogonal to the plane
  public:
    Plane();
    Plane(Point p0, Vector3 n);
    Point get_p0();
    Vector3 get_n();
    void set_params(Point* p0, Vector3* n);
};

#endif

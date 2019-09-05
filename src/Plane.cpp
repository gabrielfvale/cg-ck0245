#include <iostream>
#include "Plane.hpp"

Plane::Plane()
{
  p0_ = Point(); // known point of the plane
  n_ = Vector3(); // unit vector perpendicular to the plane
}
Plane::Plane(Point p0, Vector3 n)
{
  p0_ = p0;
  n_ = n;
  n_.normalize();
}

Point Plane::get_p0() { return p0_; }
Vector3 Plane::get_n() { return n_; }

void Plane::set_params(Point* p0, Vector3* n)
{
  p0_ = *p0;
  n_ = *n;
}

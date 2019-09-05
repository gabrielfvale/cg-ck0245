#include <iostream>
#include "Sphere.hpp"

Sphere::Sphere()
{
  p0_ = Point(0, 1, 0); // known point of the plane
  center_ = Point(); // unit vector perpendicular to the plane
  radius_ = 1;
}
Sphere::Sphere(Point p0, Point center, float radius)
{
  p0_ = p0;
  center_ = center;
  radius_ = radius;
}

Point Sphere::get_p0() { return p0_; }
Point Sphere::get_center() { return center_; }
float Sphere::get_radius() { return radius_; }

void Sphere::set_params(Point* p0, Point* center, float* radius)
{
  p0_ = *p0;
  center_ = *center;
  radius_ = *radius;
}

#include "Cylinder.hpp"

Cylinder::Cylinder()
{
  p0_ = Point(0, 1, 0);
  b_ = Point();
  u_ = Vector3(0, 1, 0);
  height_ = 1;
  radius_ = 1;
  u_.normalize();
}
Cylinder::Cylinder(Point p0, Point b, Vector3 u, float height, float radius)
{
  p0_ = p0; b_ = b; u_ = u; height_ =  height; radius_ = radius;
}
Point* Cylinder::get_p0() { return &p0_; }
Point* Cylinder::get_center() { return &b_; }
Vector3* Cylinder::get_axis() { return &u_; }
float* Cylinder::get_radius() { return &radius_; }
float* Cylinder::get_height() { return &height_; }
void Cylinder::set_params(Point* p0, Point* b, Vector3* u, float* height, float* radius)
{
  p0_ = *p0;
  b_ = *b;
  u_ = *u;
  height_ = *height;
  radius_ = *radius;
}

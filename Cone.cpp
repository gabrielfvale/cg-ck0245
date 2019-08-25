#include "Cone.h"

Cone::Cone()
{
  p0_ = Point(1, 0, 0);
  vertice_ = Point(0, 1, 0);
  n_ = Vector3(0, 1, 0);
  height_ = 1;
  radius_ = 1;
  n_.normalize();
}
Cone::Cone(Point p0, Vector3 n, float height, float radius)
{
  float vx, vy, vz;
  n.get_coordinates(&vx, &vy, &vz);
  p0_ = p0;
  vertice_ = Point(vx * height, vy * height, vz * height);
  n_ = n;
  height_ = height;
  radius_ = radius;
}
Point* Cone::get_p0() { return &p0_; }
Point* Cone::get_vertice() { return &vertice_; }
Vector3* Cone::get_axis() { return &n_; }
float* Cone::get_height() { return &height_; }
float* Cone::get_radius() { return &radius_; }
void Cone::set_params(Point* p0, Point* v, Vector3* n, float* height, float* radius)
{
  p0_ = *p0; vertice_ = *v; n_ = *n; height_ = *height; radius_ = *radius;
}

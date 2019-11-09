#include <iostream>
#include "Plane.hpp"

Plane::Plane() : Solid()
{
  p0_ = Point(); // known point of the plane
  n_ = Vector3(); // unit vector perpendicular to the plane
}
Plane::Plane(Point p0, Vector3 n) : Solid()
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

bool Plane::intersects(Ray& ray, float& t_int)
{
  Point ray_p0 = ray.get_p0();
  Vector3 d = ray.get_d();
  if(d.dot_product(&n_) == 0)
    return false;
  Vector3 resultant = Vector3(&ray_p0, &p0_);
  t_int = resultant.dot_product(&n_) / d.dot_product(&n_);
  return true;
}

void Plane::transform(Matrix4 t_matrix, TransformType t_type)
{
  p0_ = t_matrix * p0_;
  n_ = t_matrix * n_;
}

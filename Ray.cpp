#include "Ray.h"

Ray::Ray()
{
  p0_ = Point();
  d_ = Vector3();
}
Ray::Ray(Point p0, Vector3 d)
{
  p0_ = p0;
  d_ = d;
  d_.normalize();
}

Point* Ray::calc_point(float t)
{
  float p1, p2, p3, d1, d2, d3;
  p0_.get_coordinates(&p1, &p2, &p3);
  d_.get_coordinates(&d1, &d2, &d3);
  return new Point(p1 + t*d1, p2 + t*d2, p3 + t*d3);
}

Point Ray::get_p0() { return p0_; }
Vector3 Ray::get_d() { return d_; }

void Ray::set_params(Point* p0, Vector3* d)
{
  p0_ = *p0;
  d_ = *d;
}

Point* Ray::intersect(Plane& plane)
{
  Point plane_p0 = plane.get_p0();
  Vector3 plane_normal = plane.get_n();
  if(d_.dot_product(&plane_normal) == 0)
    return NULL;
  Vector3 resultant = Vector3(&p0_, &plane_p0);
  float t_int = resultant.dot_product(&plane_normal) / d_.dot_product(&plane_normal);
  return calc_point(t_int);
}

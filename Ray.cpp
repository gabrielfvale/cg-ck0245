#include <cmath>
#include <vector>
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

Point Ray::calc_point(float t)
{
  float p1, p2, p3, d1, d2, d3;
  p0_.get_coordinates(&p1, &p2, &p3);
  d_.get_coordinates(&d1, &d2, &d3);
  return Point(p1 + t*d1, p2 + t*d2, p3 + t*d3);
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
  Point i = calc_point(t_int);
  return new Point(i.get_x(), i.get_y(), i.get_z());
}
std::vector<Point> Ray::intersect(Sphere& sphere)
{
  Point sphere_center = sphere.get_center();
  Vector3 cp0 = Vector3(&sphere_center, &p0_);
  float a = d_.dot_product(&d_);
  float b = cp0.dot_product(&d_);
  float c = cp0.dot_product(&cp0) - std::pow(sphere.get_radius(), 2);
  float delta = std::pow(b, 2) - a*c;
  std::vector<Point> intersections;
  if(delta < 0)
    return intersections;
  float t_int0 = (-1 * b + std::sqrt(delta)) / a;
  float t_int1 = (-1 * b - std::sqrt(delta)) / a;
  intersections.push_back(calc_point(t_int0));
  intersections.push_back(calc_point(t_int1));
  return intersections;
}

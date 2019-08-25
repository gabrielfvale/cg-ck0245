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
std::vector<Point> Ray::intersect(Cylinder& cylinder)
{
  // v = (P0 - B) - ((P0 - B).u)u
  // w = d - (d . u)u
  float* cylinder_radius = cylinder.get_radius();
  float* cylinder_height = cylinder.get_height();
  Point* cylinder_center = cylinder.get_center();
  Vector3* cylinder_axis = cylinder.get_axis();
  Vector3 bp0 = Vector3(cylinder_center, &p0_);

  Vector3 section1 = (*cylinder_axis) * bp0.dot_product(cylinder_axis);
  Vector3 v = bp0 - section1;
  Vector3 section2 = (*cylinder_axis * d_.dot_product(cylinder_axis));
  Vector3 w = d_ - section2;

  float a = w.dot_product(&w);
  float b = v.dot_product(&w);
  float c = v.dot_product(&v) - std::pow(*cylinder_radius, 2);
  float delta = std::pow(b, 2) - a*c;
  std::vector<Point> intersections;
  if(delta < 0)
    return intersections;
  float t_int0 = (-1 * b + std::sqrt(delta)) / a;
  float t_int1 = (-1 * b - std::sqrt(delta)) / a;
  Point p1 = calc_point(t_int0);
  Point p2 = calc_point(t_int1);
  float p1_dotproduct = Vector3(cylinder_center, &p1).dot_product(cylinder_axis);
  float p2_dotproduct = Vector3(cylinder_center, &p2).dot_product(cylinder_axis);
  if(0 <= p1_dotproduct && p1_dotproduct <= *cylinder_height)
    intersections.push_back(p1);
  if(0 <= p2_dotproduct && p2_dotproduct <= *cylinder_height)
    intersections.push_back(p2);
  // no intersections with cylinder surface
  // yet there may have intersections with the caps
  if(intersections.size() == 0) {
    float x, y, z;
    (*cylinder_axis).get_coordinates(&x, &y, &z);
    Point top_center = Point(x * (*cylinder_height), y * (*cylinder_height), z * (*cylinder_height));
    Plane base_plane = Plane(*cylinder_center, *cylinder_axis);
    Plane top_plane = Plane(top_center, *cylinder_axis);
    Point* base_intersection = this->intersect(base_plane);
    Point* top_intersection = this->intersect(top_plane);
    Vector3 cbase = Vector3(cylinder_center, base_intersection);
    Vector3 ctop = Vector3(&top_center, top_intersection);
    if(cbase.norm() < *cylinder_radius)
    {
      float x, y, z;
      (*base_intersection).get_coordinates(&x, &y, &z);
      intersections.push_back(Point(x, y, z));
    }
    if(ctop.norm() < *cylinder_radius)
    {
      float x, y, z;
      (*top_intersection).get_coordinates(&x, &y, &z);
      intersections.push_back(Point(x, y, z));
    }
  }
  return intersections;
}

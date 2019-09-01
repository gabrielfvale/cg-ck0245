#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include "Ray.h"

using std::min; using std::max;

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

bool Ray::intersect(Plane& plane, float& t_int)
{
  Point plane_p0 = plane.get_p0();
  Vector3 plane_normal = plane.get_n();
  if(d_.dot_product(&plane_normal) == 0)
    return false;
  Vector3 resultant = Vector3(&p0_, &plane_p0);
  t_int = resultant.dot_product(&plane_normal) / d_.dot_product(&plane_normal);
  return true;
}

bool Ray::intersect(Sphere& sphere, float& t_min)
{
  Point sphere_center = sphere.get_center();
  Vector3 cp0 = Vector3(&sphere_center, &p0_);
  float a = d_.dot_product(&d_);
  float b = cp0.dot_product(&d_);
  float c = cp0.dot_product(&cp0) - std::pow(sphere.get_radius(), 2);
  float delta = std::pow(b, 2) - a*c;
  if(delta < 0)
    return false;
  float t_int0 = (-1 * b + std::sqrt(delta)) / a;
  float t_int1 = (-1 * b - std::sqrt(delta)) / a;
  t_min = t_int0 < t_int1 ? t_int0 : t_int1;
  return true;
}

bool Ray::intersect(Cylinder& cylinder, float& t_min)
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
  if(delta < 0)
    return false;

  float t_int0 = (-1 * b + std::sqrt(delta)) / a;
  float t_int1 = (-1 * b - std::sqrt(delta)) / a;
  Point p1 = calc_point(t_int0);
  Point p2 = calc_point(t_int1);
  float p1_dotproduct = Vector3(cylinder_center, &p1).dot_product(cylinder_axis);
  float p2_dotproduct = Vector3(cylinder_center, &p2).dot_product(cylinder_axis);

  int total_intersections = 0;
  if(0 <= p1_dotproduct && p1_dotproduct <= *cylinder_height)
  {
    t_min = t_int0;
    total_intersections++;
  }
  if(0 <= p2_dotproduct && p2_dotproduct <= *cylinder_height)
  {
    t_min = t_int1 < t_int0 ? t_int1 : t_int0;
    total_intersections++;
  }
  // no/one intersections with cylinder surface
  // yet there may have intersections with the caps
  if(total_intersections < 2)
  {
    float x, y, z;
    (*cylinder_axis).get_coordinates(&x, &y, &z);
    Point top_center = Point(x * (*cylinder_height), y * (*cylinder_height), z * (*cylinder_height));
    Plane base_plane = Plane(*cylinder_center, *cylinder_axis);
    Plane top_plane = Plane(top_center, *cylinder_axis);
    float t_base, t_top;
    bool base_intersection = this->intersect(base_plane, t_base);
    bool top_intersection = this->intersect(top_plane, t_top);
    if(base_intersection)
    {
      Point p_base = calc_point(t_base);
      Vector3 cbase = Vector3(cylinder_center, &p_base);
      if(cbase.norm() < *cylinder_radius)
      {
        t_min = t_min < t_base ? t_min : t_base;
        total_intersections++;
      }
    }
    if(top_intersection)
    {
      Point p_base = calc_point(t_top);
      Vector3 ctop = Vector3(cylinder_center, &p_base);
      if(ctop.norm() < *cylinder_radius)
      {
        t_min = t_min < t_top ? t_min : t_top;
        total_intersections++;
      }
    }
  }
  return total_intersections >= 1;
}

bool Ray::intersect(Cone& cone, float& t_min)
{
  Point* cone_vertice = cone.get_vertice();
  Vector3* axis = cone.get_axis();
  float* cone_height = cone.get_height();
  float* cone_radius = cone.get_radius();
  // v = V - P0
  Vector3 v = Vector3(&p0_, cone_vertice);
  // cos_theta = adj / sqrt(gˆ2 + rˆ2)
  // using cos_theta ^ 2
  float cos_sqrd_theta = std::pow(*cone_height / std::sqrt(std::pow(*cone_height, 2) + std::pow(*cone_radius, 2)), 2);
  float a = std::pow(d_.dot_product(axis), 2) - (d_.dot_product(&d_) * cos_sqrd_theta);
  float b = (v.dot_product(&d_) * cos_sqrd_theta) - (v.dot_product(axis) * d_.dot_product(axis));
  float c= std::pow(v.dot_product(axis), 2) - (v.dot_product(&v) * cos_sqrd_theta);
  float delta = std::pow(b, 2) - a*c;
  if(delta < 0)
    return false;
  float t_int0 = (-1 * b + std::sqrt(delta)) / a;
  float t_int1 = (-1 * b - std::sqrt(delta)) / a;
  Point p1 = calc_point(t_int0);
  Point p2 = calc_point(t_int1);
  float p1_dotproduct = Vector3(&p1, cone_vertice).dot_product(axis);
  float p2_dotproduct = Vector3(&p2, cone_vertice).dot_product(axis);

  int total_intersections = 0;
  if(0 <= p1_dotproduct && p1_dotproduct <= *cone_height)
  {
    t_min = t_int0;
    total_intersections++;
  }
  if(0 <= p2_dotproduct && p2_dotproduct <= *cone_height)
  {
    t_min = t_int1 < t_int0 ? t_int1 : t_int0;
    total_intersections++;
  }
  // one intersection with cone surface
  // the other might happen with the base
  if(delta > 0 && total_intersections == 1)
  {
    float ox, oy, oz;
    (*cone_vertice).get_coordinates(&ox, &oy, &oz);
    ox -= *cone_height * (*axis).get_x();
    oy -= *cone_height * (*axis).get_y();
    oz -= *cone_height * (*axis).get_z();
    Point base_center = Point(ox, oy, oz);
    Plane base_plane = Plane(base_center, *axis);
    float t_base;
    bool base_intersection = this->intersect(base_plane, t_base);
    if(base_intersection)
    {
      Point p_base = calc_point(t_base);
      Vector3 cbase = Vector3(&base_center, &p_base);
      if(cbase.norm() < *cone_radius)
      {
        t_min = t_min < t_base ? t_min : t_base;
        total_intersections++;
      }
    }
  }
  return total_intersections >= 1;
}

bool Ray::intersect(AABB& aabb, float& t_min)
{
  Point* cube_center = aabb.get_center();
  float* cube_edge = aabb.get_edge();
  float half_edge = (*cube_edge)/2;
  float cx, cy, cz;
  (*cube_center).get_coordinates(&cx, &cy, &cz);
  Vector3 min_bound = Vector3(cx - half_edge, cy, cz - half_edge);
  Vector3 max_bound = Vector3(cx + half_edge, cy + *cube_edge, cz + half_edge);
  // possible points
  float t0x, t0y, t0z;
  float t1x, t1y, t1z;
  /*
  t0x = (max_bound.x - p0.x) / d.x
  t1x = (min_bound.x - p0.x) / d.x
  t0y = (max_bound.y - p0.y) / d.y
  t1y = (min_bound.y - p0.y) / d.y
  t0z = (max_bound.z - p0.z) / d.z
  t1z = (min_bound.z - p0.z) / d.z
   */
  t0x = (max_bound.get_x() - p0_.get_x()) / d_.get_x();
  t1x = (min_bound.get_x() - p0_.get_x()) / d_.get_x();
  t0y = (max_bound.get_y() - p0_.get_y()) / d_.get_y();
  t1y = (min_bound.get_y() - p0_.get_y()) / d_.get_y();
  t0z = (max_bound.get_z() - p0_.get_z()) / d_.get_z();
  t1z = (min_bound.get_z() - p0_.get_z()) / d_.get_z();
  float tmin = max(max(min(t0x, t1x), min(t0y, t1y)), min(t0z, t1z));
  float tmax = min(min(max(t0x, t1x), max(t0y, t1y)), max(t0z, t1z));
  if(tmin > tmax)
    return false;
  t_min = tmin;
  return true;
}

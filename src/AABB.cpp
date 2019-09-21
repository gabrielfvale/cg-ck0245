#include "AABB.hpp"
#include <cmath>
#include <algorithm>

using std::min; using std::max;

AABB::AABB() : Object()
{
  center_ = Point();
  n_ = Vector3(0, 1, 0);
  edge_ = 1;
}
AABB::AABB(Point center, Vector3 n, float edge, Material* material) : Object(material)
{
  center_ = center;
  n_ = n;
  edge_ = edge;
  n_.normalize();
}
Point* AABB::get_center() { return &center_; }
Vector3* AABB::get_axis() { return &n_; }
float* AABB::get_edge() { return &edge_; }

void AABB::set_params(Point* center, Vector3* n, float* edge)
{
  center_ = *center;
  n_ = *n;
  edge_ = *edge;
}

Vector3 AABB::surface_normal(Point& p_int)
{
  float cx, cy, cz;
  center_.get_coordinates(&cx, &cy, &cz);
  Vector3 min_bound = Vector3(cx - edge_/2, cy, cz - edge_/2);
  Vector3 max_bound = Vector3(cx + edge_/2, cy + edge_, cz + edge_/2);

  Vector3 c = (min_bound + max_bound) * 0.5;
  Point center = Point(c.get_x(), c.get_y(), c.get_z());

  Vector3 center_p = Vector3(&center, &p_int);
  Vector3 d = (min_bound - max_bound) * 0.5;
  Vector3 normal = Vector3(
    center_p.get_x() / abs(d.get_x()),
    center_p.get_y() / abs(d.get_y()),
    center_p.get_z() / abs(d.get_z())
  );
  normal.normalize();
  return normal;
}

bool AABB::intersects(Ray& ray, float& t_min)
{
  float half_edge = edge_/2;
  float cx, cy, cz;
  center_.get_coordinates(&cx, &cy, &cz);
  Vector3 min_bound = Vector3(cx - half_edge, cy, cz - half_edge);
  Vector3 max_bound = Vector3(cx + half_edge, cy + edge_, cz + half_edge);
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
  Point p0 = ray.get_p0();
  Vector3 d = ray.get_d();
  t0x = (max_bound.get_x() - p0.get_x()) / d.get_x();
  t1x = (min_bound.get_x() - p0.get_x()) / d.get_x();
  t0y = (max_bound.get_y() - p0.get_y()) / d.get_y();
  t1y = (min_bound.get_y() - p0.get_y()) / d.get_y();
  t0z = (max_bound.get_z() - p0.get_z()) / d.get_z();
  t1z = (min_bound.get_z() - p0.get_z()) / d.get_z();
  float tmin = max(max(min(t0x, t1x), min(t0y, t1y)), min(t0z, t1z));
  float tmax = min(min(max(t0x, t1x), max(t0y, t1y)), max(t0z, t1z));
  if(tmin > tmax)
    return false;
  t_min = tmin;
  return true;
}

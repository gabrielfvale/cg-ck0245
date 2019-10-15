#include "AABB.hpp"
#include <cmath>
#include <algorithm>

using std::min; using std::max;

AABB::AABB() : Solid()
{
  center_ = Point();
  n_ = Vector3(0, 1, 0);
  edge_ = 1;
  min_bound = Point(-edge_/2, 0, -edge_/2);
  max_bound = Point(edge_/2, edge_, edge_/2);
}
AABB::AABB(Point center, Vector3 n, float edge, Material* material) : Solid(material)
{
  center_ = center;
  n_ = n;
  edge_ = edge;

  float cx, cy, cz;
  center_.get_coordinates(&cx, &cy, &cz);
  min_bound = Point(cx - edge_/2, cy, cz - edge_/2);
  max_bound = Point(cx + edge_/2, cy + edge_, cz + edge_/2);

  n_.normalize();
}
AABB::AABB(Point center, Vector3 n, float edge, Vector3 scale) : Solid()
{
  center_ = center;
  n_ = n;
  edge_ = edge;

  float cx, cy, cz;
  center_.get_coordinates(&cx, &cy, &cz);
  min_bound = Point(cx - edge_/2, cy, cz - edge_/2);
  max_bound = Point(cx + edge_/2 + scale.get_x(), cy + edge_ + scale.get_y(), cz + edge_/2 + scale.get_z());

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
  Point center = (min_bound + max_bound) * 0.5;

  Vector3 center_p = Vector3(&center, &p_int);
  Point d = (min_bound - max_bound) * 0.5;
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

void AABB::transform(Matrix4 t_matrix)
{
  center_ = t_matrix * center_;
  max_bound = t_matrix * max_bound;
  min_bound = t_matrix * min_bound;
}

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
  cx += n_.get_x()*edge_;
  cy += n_.get_y()*edge_;
  cy += n_.get_z()*edge_;
  Point c = Point(cx, cy, cz);
  Point vmin = Point(cx - edge_/2, cy - edge_/2, cz - edge_/2);
  Point vmax = Point(cx + edge_/2, cy + edge_/2, cz + edge_/2);

  Vector3 p = Vector3(&c, &p_int);

  Point d = Point(
    (vmin.get_x() - vmax.get_x())/2,
    (vmin.get_y() - vmax.get_y())/2,
    (vmin.get_z() - vmax.get_z())/2
  );
  Vector3 normal = Vector3(
    p.get_x()/std::abs(d.get_x()),
    p.get_y()/std::abs(d.get_y()),
    p.get_z()/std::abs(d.get_z())
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

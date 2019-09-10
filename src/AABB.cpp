#include "AABB.hpp"

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
    p.get_x()/abs(d.get_x()),
    p.get_y()/abs(d.get_y()),
    p.get_z()/abs(d.get_z())
  );
  normal.normalize();
  return normal;
}

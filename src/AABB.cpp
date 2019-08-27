#include "AABB.h"

AABB::AABB()
{
  center_ = Point();
  n_ = Vector3(0, 1, 0);
  edge_ = 1;
}
AABB::AABB(Point center, Vector3 n, float edge)
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

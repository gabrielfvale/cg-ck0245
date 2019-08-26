#include "OBB.h"

Cube::Cube()
{
  center_ = Point();
  u_ = Vector3(1, 0, 0);
  v_ = Vector3(0, 1, 0);
  w_ = Vector3(0, 0, 1);
  edge_ = 2;
}
Cube::Cube(Point center, Vector3 u, Vector3 v, float edge)
{
  center_ = center;
  u_ = u;
  v_ = v;
  u_.normalize();
  v_.normalize();
  w_ = u.cross_product(&v);
  edge_ = edge;
}
Point* Cube::get_center() { return &center_; }
void Cube::get_axis(Vector3* u, Vector3* v, Vector3* w)
{
  *u = u_; *v = v_; *w = w_;
}
float* Cube::get_edge() { return &edge_; }
void Cube::set_params(Point* center, float* edge)
{
  center_ = *center;
  edge_ = *edge;
}

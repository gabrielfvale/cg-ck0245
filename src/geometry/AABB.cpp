#include "AABB.hpp"
#include <cmath>
#include <limits>

using std::swap;

AABB::AABB() : Solid()
{
  center_ = Point();
  edge_ = 1;
  min_bound = Point(-edge_/2, 0, -edge_/2);
  max_bound = Point(edge_/2, edge_, edge_/2);
}
AABB::AABB(Point center, float edge, Material* material) : Solid(material)
{
  center_ = center;
  edge_ = edge;

  float cx, cy, cz;
  center_.get_coordinates(&cx, &cy, &cz);
  min_bound = Point(cx - edge_/2, cy, cz - edge_/2);
  max_bound = Point(cx + edge_/2, cy + edge_, cz + edge_/2);
  this->model_matrix.identity();
  this->inv_matrix = model_matrix;
}
AABB::AABB(Point min_point, Point max_point, Material* material) : Solid(material)
{
  edge_ = 1;
  min_bound = min_point;
  max_bound = max_point;
  center_ = (min_bound + max_bound) * 0.5;
  this->model_matrix.identity();
  this->inv_matrix = model_matrix;
}
Point* AABB::get_center() { return &center_; }
float* AABB::get_edge() { return &edge_; }

void AABB::set_params(Point* center, Vector3* n, float* edge)
{
  center_ = *center;
  edge_ = *edge;
}

Vector3 AABB::surface_normal(Point& p_int)
{
  Point center = (min_bound + max_bound) * 0.5;

  Vector3 center_p = Vector3(&center, &p_int);
  Point d = (min_bound - max_bound) * 0.5;
  Vector3 normal = Vector3(
    center_p.get_x() / std::fabs(d.get_x()),
    center_p.get_y() / std::fabs(d.get_y()),
    center_p.get_z() / std::fabs(d.get_z())
  );
  normal = model_matrix * normal;
  normal.normalize();
  return normal;
}

bool AABB::intersects(Ray& ray, float& t_min)
{
  float tmin = 0.0f;
  float tmax = 100000.0f;
  Matrix4 invm = this->inv_matrix;

  Point ray_p0 = ray.get_p0();
  Vector3 ray_d = ray.get_d();
  // Projects ray into object space
  ray_p0 = invm * ray_p0;
  ray_d = invm * ray_d;
  ray_d.normalize();

  float min_b[3], max_b[3], p0[3], d[3];
  min_bound.to_float(min_b);
  max_bound.to_float(max_b);
  ray_p0.to_float(p0);
  ray_d.to_float(d);

  // Checks intersection for each plane
  for(int i =0; i < 3; i++)
  {
    float t1 = (max_b[i] - p0[i]) / d[i];
    float t2 = (min_b[i] - p0[i]) / d[i];
    if(t1 > t2)
      swap(t1, t2);
    if(t2 < tmax)
      tmax = t2;
    if(t1 > tmin)
      tmin = t1;
    if(tmax < tmin)
      return false;
  }
  t_min = tmin;
  return true;
}

void AABB::transform(Matrix4 t_matrix, TransformType t_type)
{
  switch (t_type)
  {
  case ORIG_TRANSLATE:
    model_matrix = t_matrix * model_matrix;
    inv_matrix = model_matrix.inverse();
  case TRANSLATE:
    center_ = t_matrix * center_;
    max_bound = t_matrix * max_bound;
    min_bound = t_matrix * min_bound;
    break;
  case SCALE:
    max_bound = t_matrix * max_bound;
    break;
  case ROTATE:
    model_matrix = t_matrix * model_matrix;
    break;
  }
}

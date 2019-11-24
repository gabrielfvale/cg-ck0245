#include "OBB.hpp"
#include <cmath>

using std::swap;

OBB::OBB() : Solid(new Material(RGB(0.5, 0.5, 0.5), RGB(), RGB()))
{
  this->min_bound = Point(-0.5, -0.5, -0.5);
  this->max_bound = Point(0.5, 0.5, 0.5);
  this->modelmatrix.identity();
}

OBB::OBB(Point min_bound, Point max_bound) : Solid(new Material(RGB(0.5, 0.5, 0.5), RGB(), RGB()))
{
  this->min_bound = min_bound;
  this->max_bound = max_bound;
  this->modelmatrix.identity();
  this->invmatrix = modelmatrix;
}

OBB::OBB(float* min_bound, float* max_bound) : Solid(new Material(RGB(0.5, 0.5, 0.5), RGB(), RGB()))
{
  this->min_bound = Point(min_bound);
  this->max_bound = Point(max_bound);
  this->modelmatrix.identity();
  this->invmatrix = modelmatrix;
}

Point OBB::get_ref()
{
  return Point(
    (max_bound.get_x() + min_bound.get_x())/2,
    (max_bound.get_y() + min_bound.get_y())/2,
    (max_bound.get_z() + min_bound.get_z())/2
  );
}

void OBB::bounds(Point& minb, Point& maxb)
{
  minb = this->min_bound;
  maxb = this->max_bound;
}
void OBB::set_bounds(Point& minb, Point& maxb)
{
  this->min_bound = minb;
  this->max_bound = maxb;
}

Vector3 OBB::surface_normal(Point& p_int) { return Vector3(); }
bool OBB::intersects(Ray& ray, float& t_int)
{
  float tmin = 0.0f;
  float tmax = 100000.0f;
  Matrix4 invm = this->invmatrix;

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
  t_int = tmin;
  return true;
}
void OBB::transform(Matrix4 t_matrix, TransformType t_type)
{
  switch (t_type)
  {
  case ORIG_TRANSLATE:
    modelmatrix = t_matrix * modelmatrix;
    invmatrix = modelmatrix.inverse();
  case TRANSLATE:
    min_bound = t_matrix * min_bound;
    max_bound = t_matrix * max_bound;
    break;
  case SCALE:
    min_bound = t_matrix * min_bound;
    max_bound = t_matrix * max_bound;
    break;
  case ROTATE:
    modelmatrix = t_matrix * modelmatrix;
    break;
  }
}

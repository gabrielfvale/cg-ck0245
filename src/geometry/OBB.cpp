#include "OBB.hpp"
#include <cmath>

OBB::OBB() : Solid()
{
  this->min_bound = Point(-0.5, -0.5, -0.5);
  this->max_bound = Point(0.5, 0.5, 0.5);
  this->modelmatrix.identity();
}

OBB::OBB(Point min_bound, Point max_bound) : Solid()
{
  this->min_bound = min_bound;
  this->max_bound = max_bound;
  this->modelmatrix.identity();
}

OBB::OBB(float* min_bound, float* max_bound) : Solid()
{
  this->min_bound = Point(min_bound);
  this->max_bound = Point(max_bound);
  this->modelmatrix.identity();
}

Vector3 OBB::surface_normal(Point& p_int) { return Vector3(); }
bool OBB::intersects(Ray& ray, float& t_int)
{
  float epsilon = 1e-6f;
  float tmin = 0.0f;
  float tmax = 100000.0f;
  Matrix4 m = this->modelmatrix;

  Point ray_p0 = ray.get_p0();
  Vector3 ray_d = ray.get_d();

  Point position = Point(m(3, 0), m(3, 1), m(3, 2));
  Vector3 delta = Vector3(&ray_p0, &position);

  // intersections with X planes
  {
    Vector3 x_axis = Vector3(m(0, 0), m(0, 1), m(0, 2));
    float e = x_axis.dot_product(&delta);
    float f = ray_d.dot_product(&x_axis);
    if(std::fabs(f) > epsilon)
    {
      float t1 = (e+min_bound.get_x())/f;
      float t2 = (e+max_bound.get_x())/f;
      if(t1 > t2)
        std::swap(t1, t2);
      if(t2 < tmax)
        tmax = t2;
      if(t1 > tmin)
        tmin = t1;
      if(tmax < tmin)
        return false;
    } else
    {
      if(-e+min_bound.get_x() > 0.0f || -e+max_bound.get_x() < 0.0f)
        return false;
    }
  }
  // intersections with Y planes
  {
    Vector3 y_axis = Vector3(m(1, 0), m(1, 1), m(1, 2));
    float e = y_axis.dot_product(&delta);
    float f = ray_d.dot_product(&y_axis);
    if(std::fabs(f) > epsilon)
    {
      float t1 = (e+min_bound.get_y())/f;
      float t2 = (e+max_bound.get_y())/f;
      if(t1 > t2)
        std::swap(t1, t2);
      if(t2 < tmax)
        tmax = t2;
      if(t1 > tmin)
        tmin = t1;
      if(tmax < tmin)
        return false;
    } else
    {
      if(-e+min_bound.get_y() > 0.0f || -e+max_bound.get_y() < 0.0f)
        return false;
    }
  }
  // intersections with Z planes
  {
    Vector3 z_axis = Vector3(m(2, 0), m(2, 1), m(2, 2));
    float e = z_axis.dot_product(&delta);
    float f = ray_d.dot_product(&z_axis);
    if(std::fabs(f) > epsilon)
    {
      float t1 = (e+min_bound.get_z())/f;
      float t2 = (e+max_bound.get_z())/f;
      if(t1 > t2)
        std::swap(t1, t2);
      if(t2 < tmax)
        tmax = t2;
      if(t1 > tmin)
        tmin = t1;
      if(tmax < tmin)
        return false;
    } else
    {
      if(-e+min_bound.get_z() > 0.0f || -e+max_bound.get_z() < 0.0f)
        return false;
    }
  }
  t_int = tmin;
  return true;
}
void OBB::transform(Matrix4 t_matrix, TransformType t_type)
{
  switch (t_type)
  {
  case TRANSLATE:
    modelmatrix = modelmatrix * t_matrix;
    min_bound = t_matrix * min_bound;
    max_bound = t_matrix * max_bound;
    break;
  case SCALE:
    min_bound = t_matrix * min_bound;
    max_bound = t_matrix * max_bound;
    break;
  case ROTATE:
    // TODO: Angles are inverted on Y-axis
    modelmatrix = modelmatrix * t_matrix;
    break;
  }
}

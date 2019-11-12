#include "OBB.hpp"
#include <cmath>

OBB::OBB(Point min_bound, Point max_bound) : Solid(new Material(RGB(1, 1, 1), RGB(1, 1, 1), RGB(1, 1, 1)))
{
  this->min_bound = min_bound;
  this->max_bound = max_bound;
  this->center = Point(
    (max_bound.get_x() - min_bound.get_x())/2,
    (max_bound.get_y() - min_bound.get_y())/2,
    (max_bound.get_z() - min_bound.get_z())/2
  );
  this->axis[0] = Vector3(1, 0, 0);
  this->axis[1] = Vector3(0, 1, 0);
  this->axis[2] = Vector3(0, 0, 1);
}

Point* OBB::get_center() { return &this->center; }
Vector3* OBB::get_axis() { return this->axis; }

Vector3 OBB::surface_normal(Point& p_int) { return Vector3(); }
bool OBB::intersects(Ray& ray, float& t_int)
{
  float epsilon = 0.001f;
  float tmin = 0.0f;
  float tmax = 100000.0f;

  Point ray_p0 = ray.get_p0();
  Vector3 ray_d = ray.get_d();

  Vector3 delta = Vector3(&ray_p0, &center);

  // intersections with X planes
  {
    Vector3 ax = this->axis[0];
    float e = ax.dot_product(&delta);
    float f = ray_d.dot_product(&ax);
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
    }
  }
  // intersections with Y planes
  {
    Vector3 ay = this->axis[1];
    float e = ay.dot_product(&delta);
    float f = ray_d.dot_product(&ay);
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
    }
  }
  // intersections with Z planes
  {
    Vector3 az = this->axis[2];
    float e = az.dot_product(&delta);
    float f = ray_d.dot_product(&az);
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
    }
  }
  t_int = tmin;
  return true;
}
void OBB::transform(Matrix4 t_matrix, TransformType t_type)
{
  for(int i = 0; i < 2; i++)
    axis[i] = t_matrix * axis[i];
}

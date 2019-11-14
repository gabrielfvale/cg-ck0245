#include "OBB.hpp"
#include <cmath>

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
}

OBB::OBB(float* min_bound, float* max_bound) : Solid(new Material(RGB(0.5, 0.5, 0.5), RGB(), RGB()))
{
  this->min_bound = Point(min_bound);
  this->max_bound = Point(max_bound);
  this->modelmatrix.identity();
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
  //float epsilon = 1e-6f;
  float tmin = 0.0f;
  float tmax = 100000.0f;
  Matrix4 m = this->modelmatrix;

  Point ray_p0 = ray.get_p0();
  Vector3 ray_d = ray.get_d();

  Point position = Point(m(3, 0), m(3, 1), m(3, 2));
  Vector3 delta = Vector3(&ray_p0, &position);

  float min_b[3];
  float max_b[3];
  min_bound.get_coordinates(&min_b[0], &min_b[1], &min_b[2]);
  max_bound.get_coordinates(&max_b[0], &max_b[1], &max_b[2]);

  for(int i =0; i < 3; i++)
  {
    Vector3 i_axis = Vector3(m(i, 0), m(i, 1), m(i, 2));
    float e = i_axis.dot_product(&delta);
    float f = ray_d.dot_product(&i_axis);
    if(f >= 0.0f || -f >= 0.0f)
    {
      float t1 = (e+min_b[i])/f;
      float t2 = (e+max_b[i])/f;
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
      if(-e+min_b[i] > 0.0f || -e+max_b[i] < 0.0f)
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

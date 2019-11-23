#include "Cylinder.hpp"
#include <cmath>

Cylinder::Cylinder() : Solid()
{
  b_ = Point();
  u_ = Vector3(0, 1, 0);
  height_ = 1;
  radius_ = 1;
  scale_aux = Point(radius_, height_, radius_);
  u_.normalize();
}
Cylinder::Cylinder(Point b, Vector3 u, float height, float radius, Material* material) : Solid(material)
{
  b_ = b; u_ = u; height_ =  height; radius_ = radius; scale_aux = Point(radius_, height_, radius_);
}
Point* Cylinder::get_center() { return &b_; }
Vector3* Cylinder::get_axis() { return &u_; }
float* Cylinder::get_radius() { return &radius_; }
float* Cylinder::get_height() { return &height_; }

void Cylinder::set_params(Point* b, Vector3* u, float* height, float* radius)
{
  b_ = *b;
  u_ = *u;
  height_ = *height;
  radius_ = *radius;
}

Vector3 Cylinder::surface_normal(Point& p_int)
{
  Vector3 s1 = u_ * Vector3(&b_, &p_int).dot_product(&u_);
  Point sp_int = Point(
    b_.get_x() + s1.get_x(),
    b_.get_y() + s1.get_y(),
    b_.get_z() + s1.get_z()
  );
  Vector3 normal = Vector3(&sp_int, &p_int);
  normal = normal/radius_;
  normal.normalize();
  return normal;
}

bool Cylinder::intersects(Ray& ray, float& t_min)
{
  // v = (P0 - B) - ((P0 - B).u)u
  // w = d - (d . u)u
  Point ray_p0 = ray.get_p0();
  Vector3 ray_d = ray.get_d();

/* TODO: Ellipsoid intersection
  Matrix4 ray_scale;
  ray_scale(0, 0) = radius_ / scale_aux.get_x();
  ray_scale(1, 1) = 1;
  ray_scale(2, 2) = radius_ / scale_aux.get_z();
  ray_scale(3, 3) = 1;

  ray_d = ray_scale * ray_d;
*/

  Vector3 bp0 = Vector3(&b_, &ray_p0);

  Vector3 section1 = u_ * bp0.dot_product(&u_);
  Vector3 v = bp0 - section1;
  Vector3 section2 = (u_ * ray_d.dot_product(&u_));
  Vector3 w = ray_d - section2;

  float a = w.dot_product(&w);
  float b = v.dot_product(&w);
  float c = v.dot_product(&v) - std::pow(radius_, 2);
  float delta = std::pow(b, 2) - a*c;
  if(delta < 0)
    return false;

  float t_int0 = (-1 * b + std::sqrt(delta)) / a;
  float t_int1 = (-1 * b - std::sqrt(delta)) / a;

  Point p1 = ray.calc_point(t_int0);
  Point p2 = ray.calc_point(t_int1);
  float p1_dotproduct = Vector3(&b_, &p1).dot_product(&u_);
  float p2_dotproduct = Vector3(&b_, &p2).dot_product(&u_);

  std::vector<float> intersections;

  if(t_int0 >= 0 && (0 <= p1_dotproduct && p1_dotproduct <= height_))
    intersections.push_back(t_int0);
  if(t_int1 >= 0 && (0 <= p2_dotproduct && p2_dotproduct <= height_))
    intersections.push_back(t_int1);

  // no/one intersections with cylinder surface
  // yet there may have intersections with the caps
  if((int)intersections.size() < 2)
  {
    float x, y, z;
    u_.get_coordinates(&x, &y, &z);
    Point top_center = Point(x * height_, y * height_, z * height_);
    Plane base_plane = Plane(b_, u_);
    Plane top_plane = Plane(top_center, u_);
    float t_base, t_top;
    bool base_intersection = base_plane.intersects(ray, t_base);
    bool top_intersection = top_plane.intersects(ray, t_top);
    if(base_intersection)
    {
      Point p_base = ray.calc_point(t_base);
      Vector3 cbase = Vector3(&b_, &p_base);
      if(t_base >= 0 && cbase.norm() < radius_)
        intersections.push_back(t_base);
    }
    if(top_intersection)
    {
      Point p_base = ray.calc_point(t_top);
      Vector3 ctop = Vector3(&b_, &p_base);
      if(t_top >= 0 && ctop.norm() < radius_)
        intersections.push_back(t_top);
    }
  }
  int int_candidates = (int) intersections.size();

  t_min = int_candidates != 0 ? intersections[0] : t_min;
  for(int i = 1; i < int_candidates; i++)
    if(intersections[i] < t_min)
      t_min = intersections[i];

  return int_candidates >= 1;
}

void Cylinder::transform(Matrix4 t_matrix, TransformType t_type)
{
  switch (t_type)
  {
  case TRANSLATE:
    b_ = t_matrix * b_;
    break;
  case SCALE:
    scale_aux = t_matrix * scale_aux;
    break;
  case ROTATE:
    break;
  }
}

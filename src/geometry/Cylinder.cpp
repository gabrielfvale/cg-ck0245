#include "Cylinder.hpp"
#include <cmath>

Cylinder::Cylinder() : Solid("Cylinder")
{
  b_ = Point();
  u_ = Vector3(0, 1, 0);
  height_ = 1;
  radius_ = 1;
  u_.normalize();
}
Cylinder::Cylinder(Point b, Vector3 u, float height, float radius, Material* material) : Solid("Cylinder", material)
{
  b_ = b; u_ = u; height_ =  height; radius_ = radius;
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
  float cx, cy, cz;
  b_.get_coordinates(&cx, &cy, &cz);

  // point is in one of the caps
  if(p_int.get_x() < cx + radius_ && p_int.get_x() > cx - radius_ && p_int.get_z() < cz + radius_ && p_int.get_z() > cz - radius_)
  {
    float epsilon = 1e-8;
    if(p_int.get_y() < cy + height_+epsilon && p_int.get_y() > cy + height_-epsilon)
      return Vector3(0, 1, 0);
    if(p_int.get_y() < cy+epsilon && p_int.get_y() > cy-epsilon)
      return Vector3(0, -1, 0);
  }
  // point is on the surface
  Point c0 = Point(cx, p_int.get_y(), cz);
  Vector3 normal = Vector3(&c0, &p_int);
  normal.normalize();
  return normal;
}

bool Cylinder::intersects(Ray& ray, float& t_min)
{
  // v = (P0 - B) - ((P0 - B).u)u
  // w = d - (d . u)u
  Point ray_p0 = ray.get_p0();
  Vector3 ray_d = ray.get_d();

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

  int total_intersections = 0;
  if(0 <= p1_dotproduct && p1_dotproduct <= height_)
  {
    t_min = t_int0;
    total_intersections++;
  }
  if(0 <= p2_dotproduct && p2_dotproduct <= height_)
  {
    t_min = t_int1 < t_int0 ? t_int1 : t_int0;
    total_intersections++;
  }
  // no/one intersections with cylinder surface
  // yet there may have intersections with the caps
  if(total_intersections < 2)
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
      if(cbase.norm() < radius_)
      {
        t_min = t_min < t_base ? t_min : t_base;
        total_intersections++;
      }
    }
    if(top_intersection)
    {
      Point p_base = ray.calc_point(t_top);
      Vector3 ctop = Vector3(&b_, &p_base);
      if(ctop.norm() < radius_)
      {
        t_min = t_min < t_top ? t_min : t_top;
        total_intersections++;
      }
    }
  }
  return total_intersections >= 1;
}

void Cylinder::transform(Matrix4 t_matrix)
{
  b_ = t_matrix * b_;
}

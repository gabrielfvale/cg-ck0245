#include <iostream>
#include <cmath>
#include "Sphere.hpp"

Sphere::Sphere() : Solid()
{
  p0_ = Point(0, 1, 0); // known point of the plane
  center_ = Point(); // unit vector perpendicular to the plane
  radius_ = 1;
}
Sphere::Sphere(Point p0, Point center, float radius, Material* material) : Solid(material)
{
  p0_ = p0;
  center_ = center;
  radius_ = radius;
}

Point Sphere::get_p0() { return p0_; }
Point Sphere::get_center() { return center_; }
float Sphere::get_radius() { return radius_; }

void Sphere::set_params(Point* p0, Point* center, float* radius)
{
  p0_ = *p0;
  center_ = *center;
  radius_ = *radius;
}

Vector3 Sphere::surface_normal(Point& p_int)
{
  Vector3 normal = Vector3(&center_, &p_int);
  normal = normal/radius_;
  normal.normalize();
  return normal;
}

bool Sphere::intersects(Ray& ray, float& t_min)
{
  Point ray_p0 = ray.get_p0();
  Vector3 ray_d = ray.get_d();
  Vector3 cp0 = Vector3(&center_, &ray_p0);
  float a = ray_d.dot_product(&ray_d);
  float b = cp0.dot_product(&ray_d);
  float c = cp0.dot_product(&cp0) - std::pow(radius_, 2);
  float delta = std::pow(b, 2) - a*c;
  if(delta < 0)
    return false;
  float t_int0 = (-1 * b + std::sqrt(delta)) / a;
  float t_int1 = (-1 * b - std::sqrt(delta)) / a;
  t_min = t_int0 < t_int1 ? t_int0 : t_int1;
  return true;
}

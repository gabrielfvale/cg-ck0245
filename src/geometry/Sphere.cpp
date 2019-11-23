#include <iostream>
#include <cmath>
#include "Sphere.hpp"

Sphere::Sphere() : Solid()
{
  center_ = Point(); // unit vector perpendicular to the plane
  radius_ = 1;
  scale_aux = Point(radius_, radius_, radius_);
}
Sphere::Sphere(Point center, float radius, Material* material) : Solid(material)
{
  center_ = center;
  radius_ = radius;
  scale_aux = Point(radius_, radius_, radius_);
}

Point Sphere::get_center() { return center_; }
float Sphere::get_radius() { return radius_; }

void Sphere::set_params(Point* center, float* radius)
{
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

/* TODO: Ellipsoid intersection
  Matrix4 ray_scale;
  ray_scale(0, 0) = radius_ / scale_aux.get_x();
  ray_scale(1, 1) = radius_ / scale_aux.get_y();
  ray_scale(2, 2) = radius_ / scale_aux.get_z();
  ray_scale(3, 3) = 1;

  ray_d = ray_scale * ray_d;
*/

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

void Sphere::transform(Matrix4 t_matrix, TransformType t_type)
{
  switch (t_type)
  {
  case TRANSLATE:
    center_ = t_matrix * center_;
    break;
  case SCALE:
    scale_aux = t_matrix * scale_aux;
    break;
  case ROTATE:
    break;
  }
}

void Sphere::uv(Point& p_int, float& u, float&v)
{
  Vector3 n = Vector3(&center_, &p_int);
  n.normalize();

	float phi = std::atan2(n.get_z(), n.get_x());
	float theta = std::asin(n.get_y());
	u = 1 - (phi + M_PI) / (2 * M_PI);
	v = (theta + M_PI_2) / M_PI;
}

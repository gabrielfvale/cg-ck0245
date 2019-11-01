#include "Solid.hpp"
#include <cmath>

Solid::Solid()
{
  this->material_ = new Material();
  this->name = "Unamed Solid";
}
Solid::Solid(const char* name)
{
  this->name = name;
  this->material_ = new Material();
}
Solid::Solid(const char* name, Material* material)
{
  this->name = name;
  this->material_ = material;
};

Material* Solid::get_material() { return material_; }

Vector3 Solid::light_direction(Light* light, Point& intersection)
{
  Vector3 ld = *(light->get_position());
  Point light_point = Point(ld.get_x(), ld.get_y(), ld.get_z());
  switch (light->type())
  {
  case AMBIENT:
    return Vector3(); break;
  case REMOTE:
    ld = ld * -1; break;
  default:
    ld = Vector3(&intersection, &light_point); break;
  }
  return ld;
}

RGB Solid::calculate_diffuse(Light* light, Point& intersection)
{
  if(light->type() == AMBIENT) return RGB();

  Vector3 ld = light_direction(light, intersection);
  RGB intensity = *(light->get_intensity());
  Vector3 normal = surface_normal(intersection);
  normal.normalize();

  float fd = normal.dot_product(&ld);
  fd = fd < 0 ? 0 : fd;

  RGB Id = intensity * (*material_).diffuse * fd;
  return Id;
}

RGB Solid::calculate_specular(Light* light, Point& observer, Point& intersection)
{
  if(light->type() == AMBIENT) return RGB();

  Vector3 ld = light_direction(light, intersection);
  RGB intensity = *(light->get_intensity());
  Vector3 normal = surface_normal(intersection);
  Vector3 v = Vector3(&intersection, &observer);
  Vector3 r = normal * normal.dot_product(&ld) * 2 - ld;
  v.normalize();
  r.normalize();

  float fs = std::pow(v.dot_product(&r), (*material_).shine);
  RGB Is = intensity * (*material_).specular * fs;
  return Is;
}

RGB Solid::calculate_color(Light* light, Point& observer, Point& intersection)
{
  if(light->type() == AMBIENT) return *(light->get_intensity()) * (*material_).ambient;
  RGB Id = calculate_diffuse(light, intersection);
  RGB Is = calculate_specular(light, observer, intersection);
  return Id + Is;
}
std::ostream& operator<<(std::ostream& stream, Solid& solid)
{
  stream << solid.name;
  return stream;
}

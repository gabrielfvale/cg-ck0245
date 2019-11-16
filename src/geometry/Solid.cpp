#include "Solid.hpp"
#include <cmath>

Solid::Solid()
{
  this->material_ = new Material();
}
Solid::Solid(Material* material)
{
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
  case SPOT:
  case REMOTE:
    ld = ld * -1; break;
  default:
    ld = Vector3(&intersection, &light_point); break;
  }
  ld.normalize();
  return ld;
}

RGB Solid::calculate_diffuse(Light* light, Point& intersection)
{
  if(light->type() == AMBIENT) return RGB();

  Vector3 ld = light_direction(light, intersection);
  RGB intensity = *(light->get_intensity(intersection));
  Vector3 normal = surface_normal(intersection);
  normal.normalize();

  float fd = normal.dot_product(&ld);
  fd = fd < 0 ? 0 : fd;

  RGB Id = intensity * material_->diffuse * fd;
  return Id;
}

RGB Solid::calculate_specular(Light* light, Point& observer, Point& intersection)
{
  if(light->type() == AMBIENT) return RGB();

  Vector3 ld = light_direction(light, intersection);
  RGB intensity = *(light->get_intensity(intersection));
  Vector3 normal = surface_normal(intersection);
  Vector3 v = Vector3(&intersection, &observer);
  Vector3 r = normal * normal.dot_product(&ld) * 2 - ld;
  v.normalize();
  r.normalize();

  float vr_product = v.dot_product(&r);
  vr_product = vr_product <= 0 ? 0.0f : vr_product;
  float fs = std::pow(vr_product, material_->shine);

  RGB Is = intensity * material_->specular * fs;
  return Is;
}

RGB Solid::calculate_color(Light* light, Point& observer, Point& intersection)
{
  if(light->type() == AMBIENT) return *(light->get_intensity(intersection)) * material_->ambient;
  RGB Id = calculate_diffuse(light, intersection);
  RGB Is = calculate_specular(light, observer, intersection);
  return Id + Is;
}

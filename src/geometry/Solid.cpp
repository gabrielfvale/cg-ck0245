#include "Solid.hpp"
#include <cmath>

Solid::Solid()
{
  material_ = new Material();
}
Solid::Solid(Material* material) : material_(material){};

Material* Solid::get_material() { return material_; }

RGB Solid::calculate_diffuse(RGB& light_intensity, Point& intersection, Vector3& light_direction)
{
  Vector3 normal = surface_normal(intersection);
  normal.normalize();
  float fd = normal.dot_product(&light_direction);
  fd = fd < 0 ? 0 : fd;
  RGB Id = light_intensity * (*material_).diffuse * fd;
  return Id;
}

RGB Solid::calculate_specular(RGB& light_intensity, Point& observer, Point& intersection, Vector3& light_direction)
{
  Vector3 normal = surface_normal(intersection);
  Vector3 v = Vector3(&intersection, &observer);
  Vector3 r = normal * normal.dot_product(&light_direction) * 2 - light_direction;
  v.normalize();
  r.normalize();
  float fs = std::pow(v.dot_product(&r), (*material_).shine);
  RGB Is = light_intensity * (*material_).specular * fs;
  return Is;
}

RGB Solid::calculate_color(Point& observer, Point& intersection, std::vector<Light*> lights)
{
  RGB Id = RGB();
  RGB Is = RGB();

  if(lights.size() == 0)
    return RGB();

  int ambient_i = 0;
  for(unsigned i = 0; i < lights.size(); i++)
  {
    Vector3 light_direction = *(lights[i]->get_position());
    Point light_point = Point(light_direction.get_x(), light_direction.get_y(), light_direction.get_z());

    switch (lights[i]->type())
    {
    case AMBIENT:
      ambient_i = i; continue; break;
    case REMOTE:
      light_direction = light_direction * -1; break;
    default:
      light_direction = Vector3(&intersection, &light_point); break;
    }

    RGB light_intensity = *(lights[i]->get_intensity());
    Id = calculate_diffuse(light_intensity, intersection, light_direction) + Id;
    Is = calculate_specular(light_intensity, observer, intersection, light_direction) + Is;
  }

  RGB color = *(lights[ambient_i]->get_intensity()) * (*material_).ambient + Id + Is;
  return color;
}

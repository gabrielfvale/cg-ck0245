#include "Object.hpp"
#include <cmath>

Object::Object()
{
  visibility_ = true;
}
Object::Object(Material* material)
{
  visibility_ = true;
  material_ = material;
}
void Object::set_visibility(bool visibility) { visibility_ = visibility; }
void Object::set_material(Material* material) {  }
bool Object::get_visibility() { return visibility_; }
Material* Object::get_material() { return material_; }

RGB Object::calculate_diffuse(RGB& light_intensity, Point& intersection, Vector3& light_direction)
{
  float fd = surface_normal(intersection).dot_product(&light_direction);
  fd = fd < 0 ? 0 : fd;
  RGB Id = light_intensity * (*material_).diffuse * fd;
  return Id;
}
RGB Object::calculate_specular(RGB& light_intensity, Point& observer, Point& intersection, Vector3& light_direction)
{
  Vector3 normal = surface_normal(intersection);
  Vector3 v = Vector3(&intersection, &observer);
  Vector3 r = normal * (light_direction.dot_product(&normal)) * 2;
  r.set_coordinates(r.get_x() - light_direction.get_x(), r.get_y() - light_direction.get_y(), r.get_z() - light_direction.get_z());
  float fs = std::pow(v.dot_product(&r), (*material_).shine);
  RGB Is = light_intensity * (*material_).specular * fs;
  return Is;
}

RGB Object::calculate_color(Point& observer, Point& intersection, Light& ambient_light, std::vector<RemoteLight>& remote_lights, std::vector<PointLight>& point_lights)
{
  RGB Id = RGB();
  RGB Is = RGB();

  for(int i = 0; i < remote_lights.size(); i++)
  {
    Vector3 light_direction = *(remote_lights[i].get_direction()) * -1;
    RGB light_intensity = *(remote_lights[i].get_intensity());
    Id = calculate_diffuse(light_intensity, intersection, light_direction) + Id;
    Is = calculate_specular(light_intensity, observer, intersection, light_direction) + Is;
  }

  for(int i = 0; i < point_lights.size(); i++)
  {
    Vector3 light_direction = Vector3(&intersection, point_lights[i].get_point());
    RGB light_intensity = *(point_lights[i].get_intensity());
    Id = calculate_diffuse(light_intensity, intersection, light_direction) + Id;
    Is = calculate_specular(light_intensity, observer, intersection, light_direction) + Is;
  }

  RGB color = (*(ambient_light.get_intensity()) * (*material_).ambient) + Id + Is;
  return color;
}

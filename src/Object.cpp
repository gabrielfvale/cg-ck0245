#include "Object.hpp"
#include <cmath>

Object::Object()
{
  visible_ = true;
}
Object::Object(Material* material)
{
  visible_ = true;
  material_ = material;
}
void Object::set_visible(bool visible) { visible_ = visible; }
void Object::set_material(Material* material) {  }
bool Object::visible() { return visible_; }
Material* Object::get_material() { return material_; }

RGB Object::calculate_diffuse(RGB& light_intensity, Point& intersection, Vector3& light_direction)
{
  Vector3 normal = surface_normal(intersection);
  normal.normalize();
  float fd = normal.dot_product(&light_direction);
  fd = fd < 0 ? 0 : fd;
  RGB Id = light_intensity * (*material_).diffuse * fd;
  return Id;
}
RGB Object::calculate_specular(RGB& light_intensity, Point& observer, Point& intersection, Vector3& light_direction)
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

RGB Object::calculate_color(Point& observer, Point& intersection, Light& ambient_light, std::vector<RemoteLight>& remote_lights, std::vector<PointLight>& point_lights)
{
  RGB Id = RGB();
  RGB Is = RGB();

  for(uint32_t i = 0; i < remote_lights.size(); i++)
  {
    Vector3 light_direction = *(remote_lights[i].get_direction()) * -1;
    RGB light_intensity = *(remote_lights[i].get_intensity());
    Id = calculate_diffuse(light_intensity, intersection, light_direction) + Id;
    Is = calculate_specular(light_intensity, observer, intersection, light_direction) + Is;
  }

  for(uint32_t i = 0; i < point_lights.size(); i++)
  {
    Vector3 light_direction = Vector3(&intersection, point_lights[i].get_point());
    RGB light_intensity = *(point_lights[i].get_intensity());
    Id = calculate_diffuse(light_intensity, intersection, light_direction) + Id;
    Is = calculate_specular(light_intensity, observer, intersection, light_direction) + Is;
  }

  RGB color = (*(ambient_light.get_intensity()) * (*material_).ambient) + Id + Is;
  return color;
}

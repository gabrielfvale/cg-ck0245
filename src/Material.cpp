#include "Material.hpp"

Material::Material()
{
  ambient_ = RGB(0, 0, 0);
  diffuse_ = RGB(0, 0, 0);
  specular_ = RGB(0, 0, 0);
}
Material::Material(RGB ambient, RGB diffuse, RGB specular)
{
  ambient_ = ambient;
  diffuse_ = diffuse;
  specular_ = specular;
}

void Material::set_ambient(float r, float g, float b)
{
  ambient_.r = r;
  ambient_.g = g;
  ambient_.b = b;
}
void Material::set_diffuse(float r, float g, float b)
{
  diffuse_.r = r;
  diffuse_.g = g;
  diffuse_.b = b;
}
void Material::set_specular(float r, float g, float b)
{
  specular_.r = r;
  specular_.g = g;
  specular_.b = b;
}

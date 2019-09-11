#include "Material.hpp"

Material::Material()
{
  ambient = RGB(0, 0, 0);
  diffuse = RGB(0, 0, 0);
  specular = RGB(0, 0, 0);
  shine = 0;
}
Material::Material(RGB ambient, RGB diffuse, RGB specular)
{
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
  this->shine = 1;
}

void Material::set_ambient(float r, float g, float b)
{
  this->ambient.r = r;
  this->ambient.g = g;
  this->ambient.b = b;
}
void Material::set_diffuse(float r, float g, float b)
{
  this->diffuse.r = r;
  this->diffuse.g = g;
  this->diffuse.b = b;
}
void Material::set_specular(float r, float g, float b)
{
  this->specular.r = r;
  this->specular.g = g;
  this->specular.b = b;
}
void Material::set_shine(float s) { shine = s; }

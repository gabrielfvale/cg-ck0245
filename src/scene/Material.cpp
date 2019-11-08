#include "Material.hpp"

Material::Material()
{
  ambient = RGB();
  diffuse = RGB();
  specular = RGB();
  shine = 0;
}
Material::Material(RGB ambient, RGB diffuse, RGB specular, float shine)
{
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
  this->shine = shine;
}

void Material::set_ambient(float* rgb)
{
  this->ambient.r = rgb[0];
  this->ambient.g = rgb[1];
  this->ambient.b = rgb[2];
}
void Material::set_diffuse(float* rgb)
{
  this->diffuse.r = rgb[0];
  this->diffuse.g = rgb[1];
  this->diffuse.b = rgb[2];
}
void Material::set_specular(float* rgb)
{
  this->specular.r = rgb[0];
  this->specular.g = rgb[1];
  this->specular.b = rgb[0];
}
void Material::set_shine(float s) { shine = s; }

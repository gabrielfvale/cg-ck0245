#include "Intensity.hpp"

Intensity::Intensity() { r = 0; g = 0; b = 0; }
Intensity::Intensity(float r, float g, float b)
{
  this->r = r;
  this->g = g;
  this->b = b;
}

Intensity Intensity::operator+(Intensity& op_intensity)
{
  Intensity new_intensity;
  new_intensity.r = r + op_intensity.r;
  new_intensity.g = g + op_intensity.g;
  new_intensity.b = b + op_intensity.b;
  return new_intensity;
}
Intensity Intensity::operator*(float& value)
{
  Intensity new_intensity;
  new_intensity.r = r * value;
  new_intensity.g = g * value;
  new_intensity.b = b * value;
  return new_intensity;
}

#include "RGB.hpp"

RGB::RGB() { r = 0; g = 0; b = 0; }
RGB::RGB(float r, float g, float b)
{
  this->r = r;
  this->g = g;
  this->b = b;
}
void RGB::limit_range(float& value)
{
  value = value < 0 ? 0 : value;
  value = value > 1 ? 1 : value;
}

RGB RGB::operator+(RGB& op_intensity)
{
  RGB new_intensity;
  new_intensity.r = r + op_intensity.r;
  new_intensity.g = g + op_intensity.g;
  new_intensity.b = b + op_intensity.b;
  limit_range(new_intensity.r);
  limit_range(new_intensity.g);
  limit_range(new_intensity.b);
  return new_intensity;
}
RGB RGB::operator*(RGB& op_intensity)
{
  RGB new_intensity;
  new_intensity.r = r * op_intensity.r;
  new_intensity.g = g * op_intensity.g;
  new_intensity.b = b * op_intensity.b;
  limit_range(new_intensity.r);
  limit_range(new_intensity.g);
  limit_range(new_intensity.b);
  return new_intensity;
}
RGB RGB::operator*(float& value)
{
  RGB new_intensity;
  new_intensity.r = r * value;
  new_intensity.g = g * value;
  new_intensity.b = b * value;
  limit_range(new_intensity.r);
  limit_range(new_intensity.g);
  limit_range(new_intensity.b);
  return new_intensity;
}

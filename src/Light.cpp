#include "Light.hpp"

Light::Light()
{
  intensity_.r = 0;
  intensity_.g = 0;
  intensity_.b = 0;
}

Light::Light(RGB intensity)
{
  intensity_ = intensity;
}

Light::Light(float r, float g, float b)
{
  intensity_.r = r;
  intensity_.g = g;
  intensity_.b = b;
}

void Light::set_intensity(RGB* intensity)
{
  intensity_ = *intensity;
}
void Light::set_intensity(float r, float b, float g)
{
  intensity_.r = r;
  intensity_.g = g;
  intensity_.b = b;
}

RGB* Light::get_intensity() { return &intensity_; }

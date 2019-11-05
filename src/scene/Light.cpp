#include "Light.hpp"

Light::Light()
{
  intensity_.r = 0;
  intensity_.g = 0;
  intensity_.b = 0;
  position_ = Vector3();
  l_type = AMBIENT;
  active_ = true;
}

Light::Light(RGB intensity, Vector3 position, LightType light_type)
{
  position_ = position;
  intensity_ = intensity;
  l_type = light_type;
  active_ = true;
}

Light::Light(float* rgb, Vector3 position, LightType light_type)
{
  position_ = position;
  intensity_.r = rgb[0];
  intensity_.g = rgb[1];
  intensity_.b = rgb[2];
  l_type = light_type;
  active_ = true;
}

void Light::set_intensity(RGB intensity)
{
  intensity_ = intensity;
}
void Light::set_intensity(float* rgb)
{
  intensity_.r = rgb[0];
  intensity_.g = rgb[1];
  intensity_.b = rgb[2];
}

RGB* Light::get_intensity()
{
  if(!active_) return new RGB();
  return &intensity_;
}
void Light::set_position(Vector3 position) { position_ = position; }
bool* Light::active() { return &active_; }
Vector3* Light::get_position() { return &position_; }
LightType Light::type() { return l_type; }

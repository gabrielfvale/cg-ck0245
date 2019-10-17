#include "Light.hpp"

Light::Light()
{
  intensity_.r = 0;
  intensity_.g = 0;
  intensity_.b = 0;
  position_ = Vector3();
  l_type = AMBIENT;
}

Light::Light(RGB intensity, Vector3 position, LightType light_type)
{
  position_ = position;
  intensity_ = intensity;
  l_type = light_type;
}

Light::Light(float r, float g, float b, Vector3 position, LightType light_type)
{
  position_ = position;
  intensity_.r = r;
  intensity_.g = g;
  intensity_.b = b;
  l_type = light_type;
}

void Light::set_intensity(RGB intensity)
{
  intensity_ = intensity;
}
void Light::set_intensity(float r, float b, float g)
{
  intensity_.r = r;
  intensity_.g = g;
  intensity_.b = b;
}

RGB* Light::get_intensity() { return &intensity_; }
void Light::set_position(Vector3 position) { position_ = position; }
Vector3* Light::get_position() { return &position_; }
LightType Light::type() { return l_type; }

#include "Light.hpp"
#include <cmath>
#include <iostream>

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

/* Spotlight */
Light::Light(float* rgb, Point position, Vector3 lookat, float angle, float falloff_angle, float focus)
{
  intensity_.r = rgb[0];
  intensity_.g = rgb[1];
  intensity_.b = rgb[2];
  position_ = lookat;
  spot_pos = position;
  spot_angle = angle;
  spot_falloff = falloff_angle;
  spot_focus = focus;
  l_type = SPOT;
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

RGB Light::get_intensity(Point& point)
{
  if(!active_) return RGB();

  if(l_type == SPOT)
  {
    Vector3 lpos_p = Vector3(&spot_pos, &point);
    lpos_p.normalize();

    float angle = std::acos(lpos_p.dot_product(&position_));
    if(angle > spot_angle + spot_falloff)
      return RGB();
    if(angle > spot_angle)
    {
      float gradient = 1 - (spot_focus*(angle - spot_angle)) / spot_falloff;
      return RGB(intensity_.r * gradient, intensity_.r * gradient, intensity_.r * gradient);
    }
  }

  return intensity_;
}
void Light::set_position(Vector3 position) { position_ = position; }
bool* Light::active() { return &active_; }
Vector3 Light::get_position() { return position_; }
void Light::set_spot(float* pos, float angle, float falloff, float focus)
{
  this->spot_pos = Point(pos);
  this->spot_angle = angle;
  this->spot_falloff = falloff;
  this->spot_focus = focus;
}
Point Light::get_spotpos()
{
  if (l_type == SPOT)
    return spot_pos;
  else
    return Point();
}
LightType Light::type() { return l_type; }

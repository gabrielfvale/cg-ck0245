#include "RemoteLight.hpp"

RemoteLight::RemoteLight() : Light()
{
  direction_ = Vector3(0, -1, 0);
}
RemoteLight::RemoteLight(Intensity intensity, Vector3 direction) : Light(intensity)
{
  direction_ = direction;
}
RemoteLight::RemoteLight(float r, float g, float b, Vector3 direction) : Light(r, g, b)
{
  direction_ = direction;
}

void RemoteLight::set_direction(Vector3* direction) { direction_ = *direction; }
Vector3* RemoteLight::get_direction() { return &direction_; }

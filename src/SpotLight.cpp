#include "SpotLight.hpp"
#include "Vector3.hpp"

SpotLight::SpotLight() : Light()
{
  p0_ = Point(0, 0, 0);
  direction_ = Vector3(0, -1, 0);
  theta_ = 3.14/6;
}
SpotLight::SpotLight(RGB intensity, Point p0, Vector3 direction, float theta) : Light(intensity)
{
  p0_ = p0;
  direction_ = direction;
  theta_ = theta;
}
SpotLight::SpotLight(float r, float g, float b, Point p0, Vector3 direction, float theta) : Light(r, g, b)
{
  p0_ = p0;
  direction_ = direction;
  theta_ = theta;
}

void SpotLight::set_point(Point* p0) { p0_ = *p0; }
void SpotLight::set_direction(Vector3* direction) { direction_ = *direction; }
void SpotLight::set_theta(float* theta) { theta_ = *theta; }
Point* SpotLight::get_point() { return &p0_; }
Vector3* SpotLight::get_direction() { return &direction_; }
float* SpotLight::get_theta() { return &theta_; }

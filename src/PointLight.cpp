#include "PointLight.hpp"

PointLight::PointLight() : Light()
{
  p0_ = Point(0, 0, 0);
}
PointLight::PointLight(Intensity intensity, Point p0) : Light(intensity)
{
  p0_ = p0;
}
PointLight::PointLight(float r, float g, float b, Point p0) : Light(r, g, b)
{
  p0_ = p0;
}
void PointLight::set_point(Point* p0)
{
  p0_ = *p0;
}
Point* PointLight::get_point()
{
  return &p0_;
}

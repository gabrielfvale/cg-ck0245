#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

#include "Light.hpp"
#include "Point.hpp"

class PointLight : public Light
{
  private:
    Point p0_;
  public:
    PointLight();
    PointLight(Intensity intensity, Point p0);
    PointLight(float r, float g, float b, Point p0);
    void set_point(Point* p0);
    Point* get_point();
};

#endif

#ifndef SPOT_LIGHT_HPP
#define SPOT_LIGHT_HPP

#include "Light.hpp"
#include "Vector3.hpp"

class SpotLight : public Light
{
  private:
    Point p0_;
    Vector3 direction_;
    float theta_;
  public:
    SpotLight();
    SpotLight(RGB intensity, Point p0, Vector3 direction, float theta);
    SpotLight(float r, float g, float b, Point p0, Vector3 direction, float theta);
    void set_point(Point* p0);
    void set_direction(Vector3* direction);
    void set_theta(float* theta);
    Point* get_point();
    Vector3* get_direction();
    float* get_theta();
};

#endif

#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Point.hpp"
#include "Vector3.hpp"

class Cylinder
{
  private:
    Point p0_; // known point of the cylinder
    Point b_; // center of cylinder's base
    Vector3 u_; // cylinder axis (unit vector)
    float height_;
    float radius_;
  public:
    Cylinder();
    Cylinder(Point p0, Point b, Vector3 u, float height, float radius);
    Point* get_p0();
    Point* get_center();
    Vector3* get_axis();
    float* get_radius();
    float* get_height();
    void set_params(Point* p0, Point* b, Vector3* u, float* height, float* radius);
};

#endif

#ifndef SPHERE_H
#define SPHERE_H

#include "Point.h"
#include "Vector3.h"

class Sphere
{
  private:
    Point p0_; // known point of the sphere
    Point center_; // center of the sphere
    float radius_;
  public:
    Sphere();
    Sphere(Point p0, Point center, float radius);
    Point get_p0();
    Point get_center();
    float get_radius();
    void set_params(Point* p0, Point* center, float* radius);
};

#endif

#ifndef RAY_H
#define RAY_H

#include <vector>
#include "Point.h"
#include "Vector3.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"

class Ray
{
  private:
    Point p0_; // known point of the ray
    Vector3 d_; // unit vector that determines the ray's direction
  public:
    Ray();
    Ray(Point p0, Vector3 d);
    Point calc_point(float t);
    Point get_p0();
    Vector3 get_d();
    void set_params(Point* p0, Vector3* d);
    Point* intersect(Plane& plane);
    std::vector<Point> intersect(Sphere& sphere);
    std::vector<Point> intersect(Cylinder& Cylinder);
};

#endif

#ifndef RAY_H
#define RAY_H

#include "Point.h"
#include "Vector3.h"

class Ray
{
  private:
    Point p0_;
    Vector3 d_;
  public:
    Ray(Point p0, Vector3 d);
    Point dest(float t);
    Point get_p0();
    Vector3 get_d();
    void set_params(Point* p0, Vector3* d);
};

#endif

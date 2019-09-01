#ifndef RAY_H
#define RAY_H

#include <vector>
#include "Point.h"
#include "Vector3.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "AABB.h"

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
    bool intersect(Plane& plane, float& t_int);
    bool intersect(Sphere& sphere, float& t_min);
    bool intersect(Cylinder& cylinder, float& t_min);
    bool intersect(Cone& cone, float& t_min);
    bool intersect(AABB& aabb, float& t_min);
};

#endif

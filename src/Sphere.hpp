#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Object.hpp"

class Sphere : public Object
{
  private:
    Point p0_; // known point of the sphere
    Point center_; // center of the sphere
    float radius_;
  public:
    Sphere();
    Sphere(Point p0, Point center, float radius, Material* material);
    Point get_p0();
    Point get_center();
    float get_radius();
    void set_params(Point* p0, Point* center, float* radius);
    Vector3 surface_normal(Point& p_int) override;
    bool intersects(Ray& ray, float& t_int) override;
};

#endif

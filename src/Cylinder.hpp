#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Plane.hpp"
#include "Object.hpp"

class Cylinder : public Object
{
  private:
    Point b_; // center of cylinder's base
    Vector3 u_; // cylinder axis (unit vector)
    float height_;
    float radius_;
  public:
    Cylinder();
    Cylinder(Point b, Vector3 u, float height, float radius, Material* material);
    Point* get_center();
    Vector3* get_axis();
    float* get_radius();
    float* get_height();
    void set_params(Point* b, Vector3* u, float* height, float* radius);
    Vector3 surface_normal(Point& p_int) override;
    bool intersects(Ray& ray, float& t_int) override;
};

#endif

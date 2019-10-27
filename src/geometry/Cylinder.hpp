#ifndef CYLINDER_HPP
#define CYLINDER_HPP

#include "Plane.hpp"
#include "Solid.hpp"

class Cylinder : public Solid
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
    void transform(Matrix4 t_matrix) override;
    virtual Cylinder* clone() const {return new Cylinder(*this);}
};

#endif
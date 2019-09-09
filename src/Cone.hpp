#ifndef CONE_HPP
#define CONE_HPP

#include "Point.hpp"
#include "Vector3.hpp"

class Cone
{
  private:
    Point c_; // kcylinder base center
    Point vertice_; // cone top vertice
    Vector3 n_; // cone axis (unit vector)
    float height_;
    float radius_;
  public:
    Cone();
    Cone(Point c, Vector3 n, float height, float radius);
    Point* get_center();
    Point* get_vertice();
    Vector3* get_axis();
    float* get_height();
    float* get_radius();
    void set_params(Point* p0, Point* v, Vector3* n, float* height, float* radius);
};

#endif
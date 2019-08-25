#ifndef CONE_H
#define CONE_H

#include "Point.h"
#include "Vector3.h"

class Cone
{
  private:
    Point p0_; // known point of the cylinder
    Point vertice_; // cone top vertice
    Vector3 n_; // cone axis (unit vector)
    float height_;
    float radius_;
  public:
    Cone();
    Cone(Point p0, Vector3 n, float height, float radius);
    Point* get_p0();
    Point* get_vertice();
    Vector3* get_axis();
    float* get_height();
    float* get_radius();
    void set_params(Point* p0, Point* v, Vector3* n, float* height, float* radius);
};

#endif

#ifndef AABB_HPP
#define AABB_HPP

#include "Solid.hpp"

class AABB : public Solid
{
  private:
    Point center_; // center of the cube
    Vector3 n_; // AABB axis (unit vector)
    float edge_; // edge length
    Vector3 max_bound;
    Vector3 min_bound;
  public:
    AABB();
    AABB(Point center, Vector3 n, float edge, Material* material);
    AABB(Point center, Vector3 n, float edge, Vector3 scale);
    Point* get_center();
    Vector3* get_axis();
    float* get_edge();
    void set_params(Point* center, Vector3* n, float* edge);
    Vector3 surface_normal(Point& p_int) override;
    bool intersects(Ray& ray, float& t_int) override;
};

#endif

#ifndef AABB_HPP
#define AABB_HPP

#include "Solid.hpp"

class AABB : public Solid
{
  private:
    Point center_; // center of the cube
    Vector3 n_; // AABB axis (unit vector)
    float edge_; // edge length
    Point max_bound;
    Point min_bound;
  public:
    AABB();
    AABB(Point center, Vector3 n, float edge, Material* material);
    AABB(Point center, Vector3 n, float edge, Vector3 scale); // Bounding box 1
    AABB(Vector3 n, Point min_point, Point max_point); // Bounding box 2
    Point* get_center();
    Vector3* get_axis();
    float* get_edge();
    void set_params(Point* center, Vector3* n, float* edge);
    Vector3 surface_normal(Point& p_int) override;
    bool intersects(Ray& ray, float& t_int) override;
    void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE) override;
    virtual AABB* clone() const {return new AABB(*this);}
};

#endif

#ifndef AABB_HPP
#define AABB_HPP

#include "Solid.hpp"

class AABB : public Solid
{
  private:
    Point center_; // center of the cube
    float edge_; // edge length
    Point max_bound;
    Point min_bound;
  public:
    AABB();
    AABB(Point center, float edge, Material* material);
    AABB(Point min_point, Point max_point, Material* material);
    Point* get_center();
    float* get_edge();
    void set_params(Point* center, Vector3* n, float* edge);
    void uv(Point& p_int, float& u, float&v) override {};
    Vector3 surface_normal(Point& p_int) override;
    bool intersects(Ray& ray, float& t_int) override;
    void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE) override;
    virtual AABB* clone() const {return new AABB(*this);}
};

#endif

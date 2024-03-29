#ifndef PLANE_HPP
#define PLANE_HPP

#include "Solid.hpp"

class Plane : public Solid
{
  private:
    Point p0_; // known point of the plane
    Vector3 n_; // unit vector orthogonal to the plane
  public:
    Plane();
    Plane(Point p0, Vector3 n);
    Point get_p0();
    Vector3 get_n();
    void set_params(Point* p0, Vector3* n);
    void uv(Point& p_int, float& u, float&v) override {};
    Vector3 surface_normal(Point& p_int) override {return n_;};
    bool intersects(Ray& ray, float& t_int) override;
    void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE) override;
    virtual Plane* clone() const {return new Plane(*this);}
};

#endif

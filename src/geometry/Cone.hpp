#ifndef CONE_HPP
#define CONE_HPP

#include "Plane.hpp"
#include "Solid.hpp"

class Cone : public Solid
{
  private:
    Point c_; // cone base center
    Point vertice_; // cone top vertice
    Vector3 n_; // cone axis (unit vector)
    float height_;
    float radius_;
    Point scale_aux;
  public:
    Cone();
    Cone(Point c, Vector3 n, float height, float radius, Material* material);
    Point* get_center();
    Point* get_vertice();
    Vector3* get_axis();
    float* get_height();
    float* get_radius();
    void set_params(Point* p0, Point* v, Vector3* n, float* height, float* radius);
    Vector3 surface_normal(Point& p_int) override;
    bool intersects(Ray& ray, float& t_int) override;
    void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE) override;
    virtual Cone* clone() const {return new Cone(*this);}
};

#endif

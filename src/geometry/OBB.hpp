#ifndef OBB_HPP
#define OBB_HPP

#include "Solid.hpp"

class OBB : public Solid
{
  private:
    Point center;
    Vector3 axis[3];
    Point max_bound;
    Point min_bound;
  public:
    OBB(Point min_bound, Point max_bound); // Bounding box 1
    Point* get_center();
    Vector3* get_axis();
    Vector3 surface_normal(Point& p_int) override;
    bool intersects(Ray& ray, float& t_int) override;
    void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE) override;
    virtual OBB* clone() const {return new OBB(*this);}
};

#endif

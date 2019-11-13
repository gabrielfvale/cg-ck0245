#ifndef OBB_HPP
#define OBB_HPP

#include "Solid.hpp"

class OBB : public Solid
{
  private:
    Point max_bound;
    Point min_bound;
    Matrix4 modelmatrix;
  public:
    OBB();
    OBB(Point min_bound, Point max_bound); // Bounding box 1
    OBB(float* min_bound, float* max_bound); // Bounding box 1
    Vector3 surface_normal(Point& p_int) override;
    bool intersects(Ray& ray, float& t_int) override;
    void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE) override;
    virtual OBB* clone() const {return new OBB(*this);}
};

#endif

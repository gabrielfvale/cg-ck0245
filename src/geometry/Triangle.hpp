#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "Solid.hpp"

class Triangle : public Solid
{
  private:
    Point vertex0;
    Point vertex1;
    Point vertex2;
  public:
    Triangle();
    Triangle(Point vertex0, Point vertex1, Point vertex2, Material* material);
    Triangle(float* vertex0, float* vertex1, float* vertex2, Material* material);
    void set_vertices(float* vertex0, float* vertex1, float* vertex2);
    Point* operator[](int& i);
    Vector3 surface_normal(Point& p_int) override;
    bool intersects(Ray& ray, float& t_int) override;
    void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE) override;
    virtual Triangle* clone() const {return new Triangle(*this);}
};

#endif

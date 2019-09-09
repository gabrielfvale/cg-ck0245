#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Material.hpp"
#include "Vector3.hpp"

class Object
{
  private:
    bool visibility_;
    Material* material_;
  public:
    Object();
    Object(Material* material);
    void set_visibility(bool visibility);
    void set_material(Material* material);
    bool get_visibility();
    Material* get_material();
    Vector3 surface_normal(Point& p_int);
};

#endif

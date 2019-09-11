#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Material.hpp"
#include "Ray.hpp"

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
    virtual Vector3 surface_normal(Point& p_int) { return Vector3(); }
    virtual bool intersects(Ray& ray, float& t_int) { return true; };
};

#endif

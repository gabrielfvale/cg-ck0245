#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>

#include "AABB.hpp"
#include "Solid.hpp"

class Object
{
  private:
    AABB bounding_box_;
    std::vector<Solid*> mesh_;
    bool visible_;
  public:
    Object(AABB bounding_box, std::vector<Solid*> mesh, bool visible = true);
    Object clone();
    void get(AABB& bb, std::vector<Solid*>& mesh);
    void set_visible(bool visible);
    bool visible();
    bool trace(Ray& ray, float& t_int, Solid** solid_hit);
    void translate(Vector3 t_vec);
};

#endif

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
    void set_visible(bool visible);
    bool visible();
    void trace(Ray& ray, float& t_min, RGB& color, Point& hole_point, Light& ambient_light, std::vector<RemoteLight>& rl, std::vector<PointLight>& pl);
    void translate(Vector3 t_vec);
};

#endif

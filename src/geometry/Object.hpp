#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>

#include "AABB.hpp"
#include "Solid.hpp"

#include "../scene/Intersection.hpp"

class Object
{
  private:
    AABB bounding_box_;
    std::vector<Solid*> mesh_;
    bool visible_;
  public:
    const char* name;
    Object(const char* name, AABB bounding_box, std::vector<Solid*> mesh, bool visible = true);
    Object(const char* name, const char* obj_path, Material* material, bool visible = true);
    Object* clone();
    void get(AABB& bb, std::vector<Solid*>& mesh);
    void set_visible(bool visible);
    bool visible();
    bool trace(Ray& ray, Intersection& intersection);
    void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE);
    void translate(Vector3 t_vec);
    void scale(float sx, float sy, float sz);
    friend std::ostream& operator<<(std::ostream& stream, Object& object);
};

#endif

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>

#include "OBB.hpp"
#include "Solid.hpp"

#include "../scene/Intersection.hpp"

class Object
{
  private:
    OBB bounding_box_;
    std::vector<Solid*> mesh_;
    bool visible_;
  public:
    const char* name;
    Object(const char* name, OBB bounding_box, std::vector<Solid*> mesh, bool visible = true);
    Object(const char* name, const char* obj_path, Material* material, bool visible = true);
    Object* clone();
    void get(OBB& bb, std::vector<Solid*>& mesh);
    void set_visible(bool visible);
    bool visible();
    bool trace(Ray& ray, Intersection& intersection);
    void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE);
    void translate(Vector3 t_vec);
    void scale(float sx, float sy, float sz);
    void rotate(float angle, Vector3 axis);
    friend std::ostream& operator<<(std::ostream& stream, Object& object);
};

#endif

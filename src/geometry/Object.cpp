#include "Object.hpp"
#include <iostream>
#include <limits>

Object::Object(AABB bounding_box, std::vector<Solid*> mesh, bool visible)
{
  this->bounding_box_ = bounding_box;
  this->visible_ = visible;
  for(unsigned i=0; i<mesh.size(); i++)
    mesh_.push_back(mesh[i]->clone());
}

void Object::get(AABB& bb, std::vector<Solid*>& mesh)
{
  bb = bounding_box_;
  mesh = mesh_;
}

Object* Object::clone()
{
  return new Object(bounding_box_, mesh_, visible_);
}

void Object::set_visible(bool visible) { visible_ = visible; }
bool Object::visible() { return visible_; }

bool Object::trace(Ray& ray, Intersection& intersection)
{
  float t_min = std::numeric_limits<float>::infinity();
  float t_int;
  bool hit = false;
/*
  if(!visible_ || !bounding_box_.intersects(ray, t_int))
    return hit;
*/

  for(unsigned i = 0; i < mesh_.size(); i++)
  {
    if(mesh_[i]->intersects(ray, t_int) && t_int >= 0 && t_int < t_min)
    {
      t_min = t_int;
      intersection.solid_hit = mesh_[i];
      intersection.index = i;
      intersection.tint = t_int;
      hit = true;
    }
  }
  return hit;
}

void Object::transform(Matrix4 t_matrix, TransformType t_type)
{
  bounding_box_.transform(t_matrix, t_type);
  for(unsigned i=0; i<mesh_.size(); i++)
    mesh_[i]->transform(t_matrix, t_type);
}

void Object::translate(Vector3 t_vec)
{
  Matrix4 translation_m = Matrix4();
  translation_m.identity();
  translation_m(0, 3) = t_vec.get_x();
  translation_m(1, 3) = t_vec.get_y();
  translation_m(2, 3) = t_vec.get_z();
  transform(translation_m);
}

void Object::scale(float sx, float sy, float sz)
{
  Matrix4 scale_m = Matrix4();
  scale_m(0, 0) = sx;
  scale_m(1, 1) = sy;
  scale_m(2, 2) = sz;
  scale_m(3, 3) = 1;
}

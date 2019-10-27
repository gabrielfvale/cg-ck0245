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

Object Object::clone()
{
  return Object(bounding_box_, mesh_, visible_);
}

void Object::set_visible(bool visible) { visible_ = visible; }
bool Object::visible() { return visible_; }

bool Object::trace(Ray& ray, Intersection& intersection, RayType ray_type, int skip_index)
{
  float t_min = std::numeric_limits<float>::infinity();
  float t_int;

  if(!visible_ || !bounding_box_.intersects(ray, t_int))
    return false;

  for(unsigned i = 0; i < mesh_.size(); i++)
  {
    if(mesh_[i]->intersects(ray, t_int) && t_int < t_min)
    {
      if(ray_type == SHADOW_RAY && skip_index == (int)i) continue;
      t_min = t_int;
      intersection.solid_hit = mesh_[i];
      intersection.index = i;
      intersection.tint = t_int;
    }
  }

  return intersection.solid_hit != NULL;
}

void Object::translate(Vector3 t_vec)
{
  Matrix4 t_matrix = Matrix4();
  t_matrix.identity();
  t_matrix(0, 3) = t_vec.get_x();
  t_matrix(1, 3) = t_vec.get_y();
  t_matrix(2, 3) = t_vec.get_z();
  bounding_box_.transform(t_matrix);
  for(unsigned i=0; i<mesh_.size(); i++)
    mesh_[i]->transform(t_matrix);
}

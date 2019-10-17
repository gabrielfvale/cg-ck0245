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

Object Object::clone()
{
  return Object(bounding_box_, mesh_, visible_);
}

void Object::set_visible(bool visible) { visible_ = visible; }
bool Object::visible() { return visible_; }

bool Object::trace(Ray& ray, float& t_int, Solid** solid_hit)
{
  float ray_tint;
  float t_min = std::numeric_limits<float>::infinity();
  Solid* curr_hit = NULL;

  if(!visible_ || !bounding_box_.intersects(ray, ray_tint))
    return false;

  for(unsigned i = 0; i < mesh_.size(); i++)
  {
    if((*mesh_[i]).intersects(ray, ray_tint))
    {
      if(curr_hit == NULL || (ray_tint < t_min))
      {
        t_min = ray_tint;
        curr_hit = mesh_[i];
      }
    }
  }

  if(curr_hit)
  {
    t_int = t_min;
    *solid_hit = curr_hit;
    return true;
  }
  return false;
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

#include "Object.hpp"
#include <iostream>

Object::Object(AABB bounding_box, std::vector<Solid*> mesh, bool visible)
{
  this->bounding_box_ = bounding_box;
  this->mesh_ = mesh;
  this->visible_ = visible;
}

void Object::set_visible(bool visible) { visible_ = visible; }
bool Object::visible() { return visible_; }

void Object::trace(Ray& ray, float& t_min, RGB& color, Point& hole_point, Light& ambient_light, std::vector<RemoteLight>& rl, std::vector<PointLight>& pl)
{
  float t_int;
  Solid* solid_hit = NULL;

  if(!visible_ || !bounding_box_.intersects(ray, t_int))
    return;

  for(unsigned i = 0; i < mesh_.size(); i++)
  {
    if((*mesh_[i]).intersects(ray, t_int))
    {
      if(solid_hit == NULL || (t_int < t_min))
      {
        t_min = t_int;
        solid_hit = mesh_[i];
      }
    }
  }

  if(solid_hit)
  {
    Point intersection = ray.calc_point(t_min);
    color = (*solid_hit).calculate_color(hole_point, intersection, ambient_light, rl, pl);
  }
}

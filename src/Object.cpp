#include "Object.hpp"

Object::Object()
{
  visibility_ = true;
}
Object::Object(Material* material)
{
  visibility_ = true;
  material_ = material;
}
void Object::set_visibility(bool visibility)
{
  visibility_ = visibility;
}
void Object::set_material(Material* material) {  }
bool Object::get_visibility() { return visibility_; }
Material* Object::get_material() { return material_; }

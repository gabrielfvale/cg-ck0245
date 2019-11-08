#include "Object.hpp"
#include "Triangle.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

Object::Object(AABB bounding_box, std::vector<Solid*> mesh, bool visible)
{
  this->bounding_box_ = bounding_box;
  this->visible_ = visible;
  for(unsigned i=0; i<mesh.size(); i++)
    mesh_.push_back(mesh[i]->clone());
}

Object::Object(char* obj_path, Material* material, bool visible)
{
  vector<Point> vertices;

  float infinity = std::numeric_limits<float>::infinity();
  float min_point[3] = {infinity, infinity, infinity};
  float max_point[3] = {-infinity, -infinity, -infinity};

  ifstream in(obj_path, ios::in);
  if(!in)
  {
    cerr << "Cannot open " << obj_path << endl;
    exit(1);
  }
  string line;
  while(getline(in, line))
  {
    if(line.substr(0, 2) == "v ")
    {
      istringstream s(line.substr(2));
      float px, py, pz;
      s >> px; s >> py; s >> pz;
      min_point[0] = std::min(min_point[0], px);
      min_point[1] = std::min(min_point[1], py);
      min_point[2] = std::min(min_point[2], pz);

      max_point[0] = std::max(max_point[0], px);
      max_point[1] = std::max(max_point[1], py);
      max_point[2] = std::max(max_point[2], pz);
      vertices.push_back(Point(px, py, pz));
    }
    else if(line.substr(0, 2) == "f ")
    {
      istringstream s(line.substr(2));
      float iv0, iv1, iv2;
      s >> iv0; s >> iv1; s >> iv2;
      iv0--; iv1--; iv2--;
      mesh_.push_back(new Triangle(vertices[iv0], vertices[iv1], vertices[iv2], material));
    }
  }
  this->visible_ = visible;
  bounding_box_ = AABB(Vector3(0, 1, 0), Point(min_point), Point(max_point));
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

  if(!visible_ || !bounding_box_.intersects(ray, t_int))
    return hit;

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

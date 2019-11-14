#include "Object.hpp"
#include "Triangle.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;


void Object::load_obj(const char* obj_path, Material* material, Point& c_min, Point& c_max, bool first_time)
{
  vector<Point> vertices;

  float infinity = numeric_limits<float>::infinity();
  float min_point[3] = {infinity, infinity, infinity};
  float max_point[3] = {-infinity, -infinity, -infinity};

  if(!first_time)
  {
    c_min.get_coordinates(&min_point[0], &min_point[1], &min_point[2]);
    c_max.get_coordinates(&max_point[0], &max_point[1], &max_point[2]);
  }

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
      min_point[0] = min(min_point[0], px);
      min_point[1] = min(min_point[1], py);
      min_point[2] = min(min_point[2], pz);

      max_point[0] = max(max_point[0], px);
      max_point[1] = max(max_point[1], py);
      max_point[2] = max(max_point[2], pz);
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
  vertices.clear();
  c_min = Point(min_point);
  c_max = Point(max_point);
}

Object::Object(const char* name, OBB bounding_box, std::vector<Solid*> mesh, bool visible)
{
  this->bounding_box_ = bounding_box;
  this->name = name;
  this->visible_ = visible;
  for(unsigned i=0; i<mesh.size(); i++)
    mesh_.push_back(mesh[i]->clone());
}

Object::Object(const char* name, const char* obj_path, Material* material, bool visible)
{
  this->name = name;
  this->visible_ = visible;
  Point minb;
  Point maxb;
  this->load_obj(obj_path, material, minb, maxb, true);
  bounding_box_ = OBB(minb, maxb);
}

void Object::get(OBB& bb, std::vector<Solid*>& mesh)
{
  bb = bounding_box_;
  mesh = mesh_;
}

Object* Object::clone()
{
  return new Object(name, bounding_box_, mesh_, visible_);
}

void Object::include(const char* obj_path, Material* material)
{
  Point c_min, c_max;
  bounding_box_.bounds(c_min, c_max);
  this->load_obj(obj_path, material, c_min, c_max);
  this->bounding_box_.set_bounds(c_min, c_max);
}

void Object::set_visible(bool visible) { visible_ = visible; }
bool Object::visible() { return visible_; }
bool* Object::visible_ptr() { return &visible_; }

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
  Matrix4 translation_m;
  translation_m.identity();
  translation_m(0, 3) = t_vec.get_x();
  translation_m(1, 3) = t_vec.get_y();
  translation_m(2, 3) = t_vec.get_z();
  transform(translation_m);
}

void Object::scale(float sx, float sy, float sz)
{
  Matrix4 scale_m;
  scale_m(0, 0) = sx;
  scale_m(1, 1) = sy;
  scale_m(2, 2) = sz;
  scale_m(3, 3) = 1;
  transform(scale_m, SCALE);
}

void Object::rotate(float angle, Vector3 axis)
{

  Point origin;
  Point ref = bounding_box_.get_ref();
  Vector3 p_orig = Vector3(&ref, &origin);
  Matrix4 to_origin;
  to_origin.identity();
  to_origin(0, 3) = p_orig.get_x();
  to_origin(1, 3) = p_orig.get_y();
  to_origin(2, 3) = p_orig.get_z();
  Matrix4 from_origin;
  from_origin.identity();
  from_origin(0, 3) = -p_orig.get_x();
  from_origin(1, 3) = -p_orig.get_y();
  from_origin(2, 3) = -p_orig.get_z();

  angle /= 2;
  Vector3 qrv = axis * std::sin(angle);
  float wq = std::cos(angle);
  float vqx, vqy, vqz;
  qrv.get_coordinates(&vqx, &vqy, &vqz);

  Matrix4 rotation_m;
  // omitted fields are set to 0 by default
  rotation_m(0, 0) = vqx*vqx - vqy*vqy - vqz*vqz + wq*wq;
  rotation_m(0, 1) = 2*vqx*vqy - 2*vqz*wq;
  rotation_m(0, 2) = 2*vqx*vqz + 2*vqy*wq;

  rotation_m(1, 0) = 2*vqz*vqy + 2*vqz*wq;
  rotation_m(1, 1) = -(vqx*vqx) + vqy*vqy - vqz*vqz + wq*wq;
  rotation_m(1, 2) = 2*vqy*vqz - 2*vqx*wq;

  rotation_m(2, 0) = 2*vqx*vqz - 2*vqy*wq;
  rotation_m(2, 1) = 2*vqy*vqz + 2*vqx*wq;
  rotation_m(2, 2) = -(vqx*vqx) - vqy*vqy + vqz*vqz + wq*wq;

  rotation_m(3, 3) = 1;

  transform(to_origin);
  transform(rotation_m, ROTATE);
  transform(from_origin);
}

std::ostream& operator<<(std::ostream& stream, Object& object)
{
  stream << "Object " << object.name << endl;
  stream << " - Shapes: " << object.mesh_.size();
  return stream;
}

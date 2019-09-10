#include "Cone.hpp"

Cone::Cone() : Object()
{
  c_ = Point(0, 0, 0);
  vertice_ = Point(0, 1, 0);
  n_ = Vector3(0, 1, 0);
  height_ = 1;
  radius_ = 1;
  n_.normalize();
}
Cone::Cone(Point c, Vector3 n, float height, float radius, Material* material) : Object(material)
{
  c_ = c;
  n_ = n;
  height_ = height;
  radius_ = radius;
  n_.normalize();
  float ox, oy, oz, vx, vy, vz;
  c_.get_coordinates(&ox, &oy, &oz);
  n_.get_coordinates(&vx, &vy, &vz);
  ox += vx * height_;
  oy += vy * height_;
  oz += vz * height_;
  vertice_ = Point(ox, oy, oz);
}
Point* Cone::get_center() { return &c_; }
Point* Cone::get_vertice() { return &vertice_; }
Vector3* Cone::get_axis() { return &n_; }
float* Cone::get_height() { return &height_; }
float* Cone::get_radius() { return &radius_; }
void Cone::set_params(Point* c, Point* v, Vector3* n, float* height, float* radius)
{
  c_ = *c; vertice_ = *v; n_ = *n; height_ = *height; radius_ = *radius;
}
Vector3 Cone::surface_normal(Point& p_int)
{
  Vector3 center_p = Vector3(&c_, &p_int);
  center_p = n_*(center_p.dot_product(&n_));
  float px, py, pz;
  c_.get_coordinates(&px, &py, &pz);
  px += center_p.get_x();
  py += center_p.get_y();
  pz += center_p.get_z();
  
  Point pe = Point(px, py, pz);
  Vector3 pep_int = Vector3(&pe, &p_int);
  Vector3 p_intv = Vector3(&p_int, &vertice_);
  Vector3 t = p_intv.cross_product(&pep_int);

  Vector3 normal = t.cross_product(&p_intv);
  normal.normalize();
  return normal;
}

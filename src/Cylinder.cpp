#include "Cylinder.hpp"

Cylinder::Cylinder() : Object()
{
  p0_ = Point(0, 1, 0);
  b_ = Point();
  u_ = Vector3(0, 1, 0);
  height_ = 1;
  radius_ = 1;
  u_.normalize();
}
Cylinder::Cylinder(Point p0, Point b, Vector3 u, float height, float radius, Material* material) : Object(material)
{
  p0_ = p0; b_ = b; u_ = u; height_ =  height; radius_ = radius;
}
Point* Cylinder::get_p0() { return &p0_; }
Point* Cylinder::get_center() { return &b_; }
Vector3* Cylinder::get_axis() { return &u_; }
float* Cylinder::get_radius() { return &radius_; }
float* Cylinder::get_height() { return &height_; }
void Cylinder::set_params(Point* p0, Point* b, Vector3* u, float* height, float* radius)
{
  p0_ = *p0;
  b_ = *b;
  u_ = *u;
  height_ = *height;
  radius_ = *radius;
}

Vector3 Cylinder::surface_normal(Point& p_int)
{
  Vector3 center_p = Vector3(&b_, &p_int);
  center_p = u_*(center_p.dot_product(&u_));
  
  float px, py, pz;
  u_.get_coordinates(&px, &py, &pz);
  px += center_p.get_x();
  py += center_p.get_y();
  pz += center_p.get_z();

  Point pe = Point(px, py, pz);
  Vector3 normal = Vector3(&pe, &p_int)/radius_;
  normal.normalize();
  return normal;
}

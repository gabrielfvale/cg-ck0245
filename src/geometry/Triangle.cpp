#include "Triangle.hpp"

Triangle::Triangle() : Solid("Triangle")
{
  vertex0 = Point(1, 0, 0);
  vertex1 = Point(-1, 0, 0);
  vertex2 = Point(0, 1, 0);
}
Triangle::Triangle(Point vertex0, Point vertex1, Point vertex2, Material* material) : Solid("Triangle", material)
{
  this->vertex0 = vertex0;
  this->vertex1 = vertex1;
  this->vertex2 = vertex2;
}
Triangle::Triangle(float* vertex0, float* vertex1, float* vertex2, Material* material) : Solid("Triangle", material)
{
  this->vertex0 = Point(vertex0);
  this->vertex1 = Point(vertex1);
  this->vertex2 = Point(vertex2);
}
void Triangle::set_vertices(float* vertex0, float* vertex1, float* vertex2)
{
  this->vertex0 = Point(vertex0);
  this->vertex1 = Point(vertex1);
  this->vertex2 = Point(vertex2);
}
Point* Triangle::operator[](int& i)
{
  switch (i)
  {
  case 0:
    return &vertex0;
    break;
  case 1:
    return &vertex1;
  case 2:
    return &vertex2;
  default:
    return NULL;
    break;
  }
}
Vector3 Triangle::surface_normal(Point& p_int)
{
  Vector3 edge1 = Vector3(&vertex0, &vertex1);
  Vector3 edge2 = Vector3(&vertex0, &vertex2);
  Vector3 normal = edge1.cross_product(&edge2);
  normal.normalize();
  return normal;
}
bool Triangle::intersects(Ray& ray, float& t_int)
{
  float episilon = 0.0001;
  // Möller–Trumbore intersection
  Point ray_p0 = ray.get_p0();
  Vector3 ray_d = ray.get_d();
  Vector3 edge1, edge2, h, s, q;
  float a, f, u, v;

  edge1 = Vector3(&vertex0, &vertex1);
  edge2 = Vector3(&vertex0, &vertex2);
  h = ray_d.cross_product(&edge2);
  a = edge1.dot_product(&h);
  if(a < episilon or std::abs(a) < episilon)
    return false;

  f = 1.0f/a;
  s = Vector3(&vertex0, &ray_p0);
  u = f * s.dot_product(&h);
  if(u < 0.0 || u > 1.0)
    return false;

  q = s.cross_product(&edge1);
  v = f * ray_d.dot_product(&q);
  if(v < 0.0 || u + v > 1.0)
    return false;

  t_int = f * edge2.dot_product(&q);
  return true;
}

void Triangle::transform(Matrix4 t_matrix, TransformType t_type)
{
  vertex0 = t_matrix * vertex0;
  vertex1 = t_matrix * vertex1;
  vertex2 = t_matrix * vertex2;
}

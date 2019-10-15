#include "Cone.hpp"
#include <cmath>

Cone::Cone() : Solid()
{
  c_ = Point(0, 0, 0);
  vertice_ = Point(0, 1, 0);
  n_ = Vector3(0, 1, 0);
  height_ = 1;
  radius_ = 1;
  n_.normalize();
}
Cone::Cone(Point c, Vector3 n, float height, float radius, Material* material) : Solid(material)
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

bool Cone::intersects(Ray& ray, float& t_min)
{
  Point p0 = ray.get_p0();
  Vector3 d = ray.get_d();
  // v = V - P0
  Vector3 v = Vector3(&p0, &vertice_);
  // cos_theta = adj / sqrt(gˆ2 + rˆ2)
  // using cos_theta ^ 2
  float cos_sqrd_theta = std::pow(height_ / std::sqrt(std::pow(height_, 2) + std::pow(radius_, 2)), 2);
  float a = std::pow(d.dot_product(&n_), 2) - (d.dot_product(&d) * cos_sqrd_theta);
  float b = (v.dot_product(&d) * cos_sqrd_theta) - (v.dot_product(&n_) * d.dot_product(&n_));
  float c= std::pow(v.dot_product(&n_), 2) - (v.dot_product(&v) * cos_sqrd_theta);
  float delta = std::pow(b, 2) - a*c;
  if(delta < 0)
    return false;
  float t_int0 = (-1 * b + std::sqrt(delta)) / a;
  float t_int1 = (-1 * b - std::sqrt(delta)) / a;
  Point p1 = ray.calc_point(t_int0);
  Point p2 = ray.calc_point(t_int1);
  float p1_dotproduct = Vector3(&p1, &vertice_).dot_product(&n_);
  float p2_dotproduct = Vector3(&p2, &vertice_).dot_product(&n_);

  int total_intersections = 0;
  if(0 <= p1_dotproduct && p1_dotproduct <= height_)
  {
    t_min = t_int0;
    total_intersections++;
  }
  if(0 <= p2_dotproduct && p2_dotproduct <= height_)
  {
    t_min = t_int1 < t_int0 ? t_int1 : t_int0;
    total_intersections++;
  }
  // one intersection with cone surface
  // the other might happen with the base
  if(delta > 0 && total_intersections == 1)
  {
    float ox, oy, oz;
    vertice_.get_coordinates(&ox, &oy, &oz);
    ox -= height_ * n_.get_x();
    oy -= height_ * n_.get_y();
    oz -= height_ * n_.get_z();
    Point base_center = Point(ox, oy, oz);
    Plane base_plane = Plane(base_center, n_);
    float t_base;
    bool base_intersection = base_plane.intersects(ray, t_base);
    if(base_intersection)
    {
      Point p_base = ray.calc_point(t_base);
      Vector3 cbase = Vector3(&base_center, &p_base);
      if(cbase.norm() < radius_)
      {
        t_min = t_min < t_base ? t_min : t_base;
        total_intersections++;
      }
    }
  }
  return total_intersections >= 1;
}

void Cone::transform(Matrix4 t_matrix)
{
  c_ = t_matrix * c_;
  vertice_ = t_matrix * vertice_;
}

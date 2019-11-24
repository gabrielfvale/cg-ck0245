#ifndef SOLID_HPP
#define SOLID_HPP

#include <iostream>

#include "../util/Material.hpp"
#include "../scene/Light.hpp"
#include "../math/Matrix4.hpp"
#include "../math/Ray.hpp"

enum TransformType { ORIG_TRANSLATE, TRANSLATE, SCALE, ROTATE };

class Solid
{
  private:
    Material* material_;
    RGB calculate_diffuse(Light* light, Point& intersection);
    RGB calculate_specular(Light* light, Point& observer, Point& intersection);
  public:
    Solid();
    Solid(Material* material);
    Material* get_material();
    Vector3 light_direction(Light* light, Point& intersection);
    RGB calculate_color(Light* light, Point& observer, Point& intersection);
    virtual void uv(Point& p_int, float& u, float&v) = 0;
    virtual Vector3 surface_normal(Point& p_int) = 0;
    virtual bool intersects(Ray& ray, float& t_int) = 0;
    virtual void transform(Matrix4 t_matrix, TransformType t_type = TRANSLATE) = 0;
    virtual Solid* clone() const = 0;
};

#endif

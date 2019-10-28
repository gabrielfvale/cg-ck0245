#ifndef SOLID_HPP
#define SOLID_HPP

#include <iostream>

#include "../scene/Material.hpp"
#include "../scene/Light.hpp"
#include "../math/Matrix4.hpp"
#include "../math/Ray.hpp"

class Solid
{
  private:
    Material* material_;
    RGB calculate_diffuse(Light* light, Point& intersection);
    RGB calculate_specular(Light* light, Point& observer, Point& intersection);
  public:
    const char* name;
    Solid();
    Solid(const char* name);
    Solid(const char* name, Material* material);
    Material* get_material();
    Vector3 light_direction(Light* light, Point& intersection);
    RGB calculate_color(Light* light, Point& observer, Point& intersection);
    virtual Vector3 surface_normal(Point& p_int) = 0;
    virtual bool intersects(Ray& ray, float& t_int) = 0;
    virtual void transform(Matrix4 t_matrix) = 0;
    virtual Solid* clone() const = 0;
    friend std::ostream& operator<<(std::ostream& stream, Solid& solid);
};

#endif

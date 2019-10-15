#ifndef SOLID_HPP
#define SOLID_HPP

#include "Material.hpp"
#include "Light.hpp"
#include "RemoteLight.hpp"
#include "PointLight.hpp"
#include "Matrix4.hpp"
#include "Ray.hpp"


class Solid
{
  private:
    Material* material_;
    RGB calculate_diffuse(RGB& light_intensity, Point& intersection, Vector3& light_direction);
    RGB calculate_specular(RGB& light_intensity, Point& observer, Point& intersection, Vector3& light_direction);
  public:
    Solid();
    Solid(Material* material);
    Material* get_material();
    RGB calculate_color(Point& observer, Point& intersection, Light& ambient_light, std::vector<RemoteLight>& remote_lights, std::vector<PointLight>& point_lights);
    virtual Vector3 surface_normal(Point& p_int) = 0;
    virtual bool intersects(Ray& ray, float& t_int) = 0;
    virtual void transform(Matrix4 t_matrix) = 0;
    virtual Solid* clone() const = 0;
};

#endif

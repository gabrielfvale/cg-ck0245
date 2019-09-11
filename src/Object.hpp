#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <vector>
#include "Material.hpp"
#include "Light.hpp"
#include "RemoteLight.hpp"
#include "PointLight.hpp"
#include "Ray.hpp"

class Object
{
  private:
    bool visibility_;
    Material* material_;
    RGB calculate_diffuse(RGB& light_intensity, Point& intersection, Vector3& light_direction);
    RGB calculate_specular(RGB& light_intensity, Point& observer, Point& intersection, Vector3& light_direction);
  public:
    Object();
    Object(Material* material);
    void set_visibility(bool visibility);
    void set_material(Material* material);
    bool get_visibility();
    Material* get_material();
    RGB calculate_color(Point& observer, Point& intersection, Light& ambient_light, std::vector<RemoteLight>& remote_lights, std::vector<PointLight>& point_lights);
    virtual Vector3 surface_normal(Point& p_int) { return Vector3(); }
    virtual bool intersects(Ray& ray, float& t_int) { return true; };
};

#endif

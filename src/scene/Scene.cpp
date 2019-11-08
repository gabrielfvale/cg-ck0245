#include "Scene.hpp"
#include <cmath>
#include <iostream>

Scene::Scene(int resolution, Camera* camera, vector<Object*> objects, vector<Light*> lights, float w, float d)
{
  this->resolution = resolution;
  this->camera = camera;
  this->objects = objects;
  this->lights = lights;
  this->width = w;
  this->distance = d;
}

bool Scene::trace(Ray& ray, Intersection& intersection)
{
  Point observer = *(camera->get_eye());
  float t_min = numeric_limits<float>::infinity();

  Intersection obj_intersect;
  int object_index = -1;

  for(unsigned i = 0; i < objects.size(); i++)
  {
    //objects[i]->transform(camera->world_to_camera());
    if(objects[i]->trace(ray, obj_intersect) && obj_intersect.tint < t_min)
    {
      t_min = obj_intersect.tint;
      intersection = obj_intersect;
      object_index = i;
    }
  }

  if(object_index != -1)
  {
    intersection.color = RGB();
    Point p_int = ray.calc_point(intersection.tint);

    // cast shadow rays for each light
    for(unsigned i = 0; i < lights.size(); i++)
    {
      if(lights[i]->type() == AMBIENT)
      {
        intersection.color += intersection.solid_hit->calculate_color(lights[i], observer, p_int);
        continue;
      }

      Vector3 light_dir = intersection.solid_hit->light_direction(lights[i], p_int);
      Ray shadowray = Ray(p_int, light_dir);

      int visible = 1;
      unsigned k = 0;
      while(visible && k < objects.size())
      {
        int skip_self = object_index == (int) k ? intersection.index : -1;
        if(objects[k]->trace(shadowray, obj_intersect, skip_self))
          visible = 0;
        k++;
      }
      intersection.color += intersection.solid_hit->calculate_color(lights[i], observer, p_int) * visible;
    }
    return true;
  }
  return false;
}

void Scene::castRay(int x, int y, Intersection& intersection)
{
  // gera o ponto da matriz em coordenadas de camera
  float pixel_width = width/resolution;
  Point observer = *(camera->get_eye());

  Point hole_point = Point(-width/2 + pixel_width/2 + x*pixel_width, width/2 - pixel_width/2 - y*pixel_width, -distance);
  // converte o ponto para coordenadas de mundo
  hole_point = camera->camera_to_world() * hole_point;
  Vector3 ray_direction = Vector3(&observer, &hole_point);
  Ray ray = Ray(observer, ray_direction);

  trace(ray, intersection);
}

void Scene::set_pixel(GLubyte* pixels, int x, int y, RGB rgb)
{
  y = resolution-1-y;
  int position = (x + y * resolution) * 3;
  pixels[position] = std::floor(rgb.r * 255);
  pixels[position + 1] = std::floor(rgb.g * 255);
  pixels[position + 2] = std::floor(rgb.b * 255);
}

void Scene::print(GLubyte* pixels)
{
  for(int y = 0; y < resolution; y++)
  {
    for (int x = 0; x < resolution; x++)
    {
      Intersection intersection;
      castRay(x, y, intersection);
      set_pixel(pixels, x, y, intersection.color);
    }
  }
}

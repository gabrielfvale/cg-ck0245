#include "Scene.hpp"
#include <cmath>

Scene::Scene(int resolution, Camera camera, vector<Object*> objects, vector<Light*> lights, float w, float d)
{
  this->resolution = resolution;
  this->camera = camera;
  this->objects = objects;
  this->lights = lights;
  this->width = w;
  this->distance = d;
}

void Scene::castRay(int x, int y, Object** object_hit, RGB& color)
{
  //std::cout << "casting ray" << std::endl;
  // gera o ponto da matriz em coordenadas de camera
  float pixel_width = width/resolution;
  Point observer = *(camera.get_eye());

  Point hole_point = Point(-width/2 + pixel_width/2 + x*pixel_width, width/2 - pixel_width/2 - y*pixel_width, -distance);
  // converte o ponto para coordenadas de mundo
  hole_point = camera.camera_to_world() * hole_point;
  Vector3 ray_direction = Vector3(&observer, &hole_point);
  Ray ray = Ray(observer, ray_direction);
  
  float t_min = numeric_limits<float>::infinity();
  float t_int;

  Solid* close_obj = NULL;
  Solid* solid_hit = NULL;

  for(unsigned i=0; i<objects.size(); i++)
  {
    if((*objects[i]).trace(ray, t_int, &solid_hit))
    {
      if(close_obj == NULL || t_int < t_min)
      {
        t_min = t_int;
        close_obj = solid_hit;
        Point intersection = ray.calc_point(t_int);

        *object_hit = objects[i];
        color = solid_hit->calculate_color(observer, intersection, lights);
      }
    }
  }
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
      RGB color = RGB();
      Object* object_hit = NULL;
      castRay(x, y, &object_hit, color);
      set_pixel(pixels, x, y, color);
    }
  }
}

#include "Scene.hpp"

#include <thread>
#include <functional>
#include <chrono>

#include <cmath>
#include <ctime>
#include <iostream>
#include <iomanip>

using namespace std;

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
    if(objects[i]->trace(ray, obj_intersect) && obj_intersect.tint < t_min)
    {
      t_min = obj_intersect.tint;
      intersection = obj_intersect;
      intersection.object_hit = objects[i];
      object_index = i;
    }
  }

  if(object_index != -1)
  {
    float episilon = 0.01;
    intersection.color = RGB();
    Point p_int = ray.calc_point(intersection.tint);
    Vector3 surfc_normal = intersection.solid_hit->surface_normal(p_int);
    surfc_normal = surfc_normal * episilon;

    // shifts intersection so there is no shadow acne
    p_int.set_coordinates(
      p_int.get_x() + surfc_normal.get_x(),
      p_int.get_y() + surfc_normal.get_y(),
      p_int.get_z() + surfc_normal.get_z()
    );

    // cast shadow rays for each light
    for(unsigned i = 0; i < lights.size(); i++)
    {
      if(! *(lights[i]->active()) ) continue;
      if(lights[i]->type() == AMBIENT)
      {
        intersection.color += intersection.solid_hit->calculate_color(lights[i], observer, p_int);
        continue;
      }

      Vector3 light_dir = intersection.solid_hit->light_direction(lights[i], p_int);
      Ray shadowray = Ray(p_int, light_dir);

      Vector3 lvp = lights[i]->get_position();
      Point light_point = lights[i]->type() == SPOT ? lights[i]->get_spotpos() : Point(lvp.get_x(), lvp.get_y(), lvp.get_z());

      int visible = 1;
      unsigned k = 0;
      while(visible && k < objects.size())
      {
        if(objects[k]->trace(shadowray, obj_intersect))
        {
          visible = 0;
          if(lights[i]->type() == POINT || lights[i]->type() == SPOT)
          {
            Point shadow_point = shadowray.calc_point(obj_intersect.tint);
            if(p_int.distance_from(&shadow_point) > p_int.distance_from(&light_point))
              visible = 1; // light is closer to the object
          }
        }
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
  float pixel_width = width/resolution;
  Point observer = *(camera->get_eye());

  float x_pos = -width/2 + pixel_width/2 + x*pixel_width;
  float y_pos = width/2 - pixel_width/2 - y*pixel_width;

  Point hole_point = Point(x_pos, y_pos, -distance);
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
  cout << "[SCENE] Started rendering" << endl;
  chrono::time_point<chrono::system_clock> render_start, render_end;

  const size_t nthreads = thread::hardware_concurrency();
  vector<thread> threads(nthreads);

  render_start = chrono::system_clock::now();

  for(size_t t = 0; t < nthreads; t++)
  {
    threads[t] = thread(bind(
    [&](const int start_i, const int end_i, const int t)
    {
    for(int y = start_i; y < end_i; y++)
    {
      for (int x = 0; x < resolution; x++)
      {
        Intersection intersection;
        castRay(x, y, intersection);
        set_pixel(pixels, x, y, intersection.color);
      }
    }
    },t*resolution/nthreads,(t+1)==nthreads?resolution:(t+1)*resolution/nthreads,t));
  }

  for(size_t i = 0; i < nthreads; i++)
    threads[i].join();

  render_end = chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = render_end - render_start;

  cout << "[SCENE] Rendered in " << setprecision(2);
  cout << elapsed_seconds.count() << "s\n"; 
}

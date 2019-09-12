#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

#include <SDL2/SDL.h>

#include "Point.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "AABB.hpp"

#include "Camera.hpp"
#include "Light.hpp"
#include "PointLight.hpp"
#include "RemoteLight.hpp"

#include "BMP.hpp"

using namespace std;

void trace_ray(int x, int y, Ray& ray, Object** objects, RGB* rgb, Light ambient_light, vector<RemoteLight> rl, vector<PointLight> pl)
{
  Point p0 = ray.get_p0();
  float t_int;
  float t_min = numeric_limits<float>::infinity();

  Object* object_hit = NULL;

  for(int i = 0; i < 7; i++)
  {
    if((*objects[i]).intersects(ray, t_int))
    {
      if(object_hit == NULL || (t_int < t_min))
      {
        t_min = t_int;
        object_hit = objects[i];
      }
    }
  }

  if(object_hit != NULL)
  {
    Point intersection = ray.calc_point(t_min);
    *rgb = (*object_hit).calculate_color(p0, intersection, ambient_light, rl, pl);
  } else
  {
    *rgb = RGB(0.6, 0.8, 1);
  }
}

int main(int argc, char *argv[])
{
  // Definições observador e placa
  int panel_holes = 500;
  float panel_l = 6;
  float panel_d = 3;

  Material dark_brown = Material(RGB(0.27, 0.13, 0), RGB(0.27, 0.13, 0), RGB());
  Material tree_green = Material(RGB(0.33, 0.49, 0.18), RGB(0.2, 0.2, 0.2), RGB());
  Material purple = Material(RGB(0.33, 0.18, 0.49), RGB(0.33, 0.18, 0.49), RGB(0.1, 0.1, 0.1));

  Point observer = Point(10, 4.5, 20);
  Point lookat = Point(10, 4.5, 5);
  Vector3 viewup = Vector3(10, 5.5, 20);

  Camera camera = Camera(observer, lookat, viewup);

  Vector3 g_axis = Vector3(0, 1, 0);
  Point cylinder_center = Point(7, 0, 9);
  Point cylinder2_center = Point(13, 0, 9);
  Point bcube_center = Point(10, 0, 5);

  // definições de objetos
  float hole_width = panel_l/panel_holes;
  float cylinder_radius = 0.5;
  float cylinder_height = 2;
  float cone_radius = 2;
  float cone_height = 8;
  float cube_edge = 3;

  // gera os objetos
  Cylinder cylinder = Cylinder(Point(cylinder_center.get_x() - cylinder_radius, cylinder_center.get_y(), cylinder_center.get_z()), cylinder_center, g_axis, cylinder_height, cylinder_radius, &dark_brown);
  Cylinder cylinder2 = Cylinder(Point(cylinder2_center.get_x() - cylinder_radius, cylinder2_center.get_y(), cylinder2_center.get_z()), cylinder2_center, g_axis, cylinder_height, cylinder_radius, &dark_brown);
  // gera os cones
  Cone cone = Cone(
    Point(
      cylinder_center.get_x() + cylinder_height * g_axis.get_x(),
      cylinder_center.get_y() + cylinder_height * g_axis.get_y(),
      cylinder_center.get_z() + cylinder_height * g_axis.get_z()
    ), g_axis, cone_height, cone_radius, &tree_green);
  Cone cone2 = Cone(
    Point(
      cylinder2_center.get_x() + cylinder_height * g_axis.get_x(),
      cylinder2_center.get_y() + cylinder_height * g_axis.get_y(),
      cylinder2_center.get_z() + cylinder_height * g_axis.get_z()
    ), g_axis, cone_height, cone_radius, &tree_green);
  // cria o cubo base
  AABB b_cube = AABB(bcube_center, g_axis, cube_edge, &purple);
  float bcx, bcy, bcz;
  bcube_center.get_coordinates(&bcx, &bcy, &bcz);
  float dx, dy, dz;
  g_axis.get_coordinates(&dx, &dy, &dz);
  // cria os demais cubos calculando o centro a partir do cubo base
  AABB m_cube = AABB(Point(bcx + (dx*cube_edge), bcy + (dy*cube_edge), bcz + (dz*cube_edge)), g_axis, cube_edge, &purple);
  AABB t_cube = AABB(Point(bcx + (2*dx*cube_edge), bcy + (2*dy*cube_edge), bcz + (2*dz*cube_edge)), g_axis, cube_edge, &purple);

  // cria as luzes
  Light ambient_light = Light(0.5, 0.5, 0.5);
  vector<RemoteLight> rl = vector<RemoteLight>();
  vector<PointLight> pl = {PointLight(RGB(0.2, 0.2, 0.2), Point(10, 15, 9))};

  Object* objects[7] = {
    &cylinder,
    &cylinder2,
    &cone,
    &cone2,
    &t_cube,
    &m_cube,
    &b_cube
  };

  //BMP preview = BMP(panel_holes, panel_holes);

  // inicia SDL
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Event event;
  SDL_CreateWindowAndRenderer(panel_holes, panel_holes, 0, &window, &renderer);
  SDL_RenderClear(renderer);

  // projeta cada um dos raios
  for(int i=0; i<panel_holes; i++)
  {
    for (int j=0; j<panel_holes; j++)
    {
      // gera o ponto da matriz em coordenadas de camera
      Point hole_point = Point(-panel_l/2 + hole_width/2 + j*hole_width, panel_l/2 - hole_width/2 - i*hole_width, -panel_d);
      // converte o ponto para coordenadas de mundo
      hole_point = camera.matrixTimesPoint(camera.camera_to_world(), hole_point);
      Vector3 ray_direction = Vector3(&observer, &hole_point);
      Ray ray = Ray(observer, ray_direction);

      float t_int;
      float t_min = numeric_limits<float>::infinity();

      Object* object_hit = NULL;

      for(int i = 0; i < 7; i++)
      {
        if((*objects[i]).intersects(ray, t_int))
        {
          if(object_hit == NULL || (t_int < t_min))
          {
            t_min = t_int;
            object_hit = objects[i];
          }
        }
      }

      if(object_hit != NULL)
      {
        Point intersection = ray.calc_point(t_min);
        RGB color = (*object_hit).calculate_color(hole_point, intersection, ambient_light, rl, pl);
        SDL_SetRenderDrawColor(renderer, floor(color.r * 255), floor(color.g * 255), floor(color.b * 255), 255);
        //preview.set_pixel(j, i, floor(color.r * 255), floor(color.g * 255), floor(color.b * 255));
      } else
      {
        SDL_SetRenderDrawColor(renderer, 153, 204, 255, 255);
        //preview.set_pixel(j, i, 153, 204, 255);
      }
      SDL_RenderDrawPoint(renderer, j, i);
    }
  }
  SDL_RenderPresent(renderer);
  bool running = true;
  while(running)
  {
    SDL_WaitEvent(&event);
    switch(event.type)
    {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_MOUSEBUTTONDOWN:
        /*
        int mouse_x = event.button.x;
        int mouse_y = event.button.x;
        cout << mouse_x << endl;
        Point hole_point = Point(-panel_l/2 + hole_width/2 + mouse_x*hole_width, panel_l/2 - hole_width/2 - mouse_y*hole_width, -panel_d);
        hole_point = camera.matrixTimesPoint(camera.camera_to_world(), hole_point);
        Ray ray = Ray(observer, Vector3(&observer, &hole_point));
        RGB return_rgb;
        trace_ray(mouse_x, mouse_y, ray, objects, &return_rgb, ambient_light, rl, pl);
        */
      break;
    }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}

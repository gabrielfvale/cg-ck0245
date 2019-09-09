#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "Point.hpp"
#include "Vector3.hpp"
#include "Ray.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Cone.hpp"
#include "AABB.hpp"

#include "Camera.hpp"

#include "BMP.hpp"

using namespace std;

int main()
{
  // Definições observador e placa
  int panel_holes;
  float panel_l, panel_d, obx, oby, obz;
  cout << "Entre a largura L da placa L*L:" << endl;
  cin >> panel_l;

  cout << "Entre a dimensão N de pontos da matriz N*N da placa:" << endl;
  cout << "(serão gerados N*N pontos que serão distribuídos igualmente pela placa)" << endl;
  cin >> panel_holes;

  cout << "Entre a distância da placa ao observador:" << endl;
  cin >> panel_d;

  // gera o ponto do observador
  cout << "Entre as coordenadas x, y e z do observador, separadas por espaços:" << endl;
  cin >> obx >> oby >> obz;
  Point observer = Point(obx, oby, obz);
  // gera o ponto lookat
  cout << "Entre as coordenadas x, y e z do lookat, separadas por espaços:" << endl;
  cin >> obx >> oby >> obz;
  Point lookat = Point(obx, oby, obz);
  // gera o vetor viewup
  cout << "Entre as coordenadas x, y e z do viewup, separadas por espaços:" << endl;
  cin >> obx >> oby >> obz;
  Vector3 viewup = Vector3(obx, oby, obz);

  // gera a camera
  Camera camera = Camera(observer, lookat, viewup);

  // gera o vetor normal a ser usado nos objetos
  Vector3 g_axis = Vector3(0, 1, 0);

  cout << "Gerando placa com " << panel_holes*panel_holes << " buracos e largura " << panel_l << "." << endl;
  float cx, cy, cz;

  cout << "Entre as coordenadas x, y e z do centro do cilindro, separadas por espaços:" << endl;
  cin >> cx >> cy >> cz;
  // cria o ponto do centro do cilindro
  Point cylinder_center = Point(cx, cy, cz);
  cout << "Entre as coordenadas x, y e z do centro do segundo cilindro, separadas por espaços:" << endl;
  cin >> cx >> cy >> cz;
  // cria o ponto do centro do cilindro
  Point cylinder2_center = Point(cx, cy, cz);

  float ojb_height, obj_radius;
  cout << "Entre a altura e o raio do cilindro, separados por espaços:" << endl;
  cin >> ojb_height >> obj_radius;
  // gera os cilindros
  Cylinder cylinder = Cylinder(Point(cylinder_center.get_x() - obj_radius, cylinder_center.get_y(), cylinder_center.get_z()), cylinder_center, g_axis, ojb_height, obj_radius);
  Cylinder cylinder2 = Cylinder(Point(cylinder2_center.get_x() - obj_radius, cylinder2_center.get_y(), cylinder2_center.get_z()), cylinder2_center, g_axis, ojb_height, obj_radius);

  cout << "Entre a altura e o raio do cone, separados por espaços:" << endl;
  cin >> ojb_height >> obj_radius;

  float* cylinder_height = cylinder.get_height();
  // gera os cones
  Cone cone = Cone(
    Point(
      cylinder_center.get_x() + (*cylinder_height) * g_axis.get_x(),
      cylinder_center.get_y() + (*cylinder_height) * g_axis.get_y(),
      cylinder_center.get_z() + (*cylinder_height) * g_axis.get_z()
    ), g_axis, ojb_height, obj_radius);
  Cone cone2 = Cone(
    Point(
      cylinder2_center.get_x() + (*cylinder_height) * g_axis.get_x(),
      cylinder2_center.get_y() + (*cylinder_height) * g_axis.get_y(),
      cylinder2_center.get_z() + (*cylinder_height) * g_axis.get_z()
    ), g_axis, ojb_height, obj_radius);

  cout << "Entre as coordenadas x, y e z do centro do primeiro cubo, separados por espaços:" << endl;
  cout << "(os demais cubos serão calculados exatamente acima do primeiro)" << endl;
  cin >> cx >> cy >> cz;
  // cria o ponto do centro do cubo mais embaixo
  Point bcube_center = Point(cx, cy, cz);

  float cube_edge;
  cout << "Entre a largura da aresta do cubo:" << endl;
  cin >> cube_edge;
  // cria o cubo base
  AABB b_cube = AABB(bcube_center, g_axis, cube_edge);
  float bcx, bcy, bcz;
  bcube_center.get_coordinates(&bcx, &bcy, &bcz);
  float dx, dy, dz;
  g_axis.get_coordinates(&dx, &dy, &dz);
  // cria os demais cubos calculando o centro a partir do cubo base
  AABB m_cube = AABB(Point(bcx + (dx*cube_edge), bcy + (dy*cube_edge), bcz + (dz*cube_edge)), g_axis, cube_edge);
  AABB t_cube = AABB(Point(bcx + (2*dx*cube_edge), bcy + (2*dy*cube_edge), bcz + (2*dz*cube_edge)), g_axis, cube_edge);

  /* debug
  cout << "CENTRO CILINDRO: " + (*(cylinder.get_center())).to_string() << endl;
  cout << "CENTRO CONE: " + (*(cone.get_center())).to_string() << endl;
  cout << "VERTICE CONE: " + (*(cone.get_vertice())).to_string() << endl;
  cout << "CENTRO BCUBE: " + (*(b_cube.get_center())).to_string() << endl;
  cout << "CENTRO MCUBE: " + (*(m_cube.get_center())).to_string() << endl;
  cout << "CENTRO TCUBE: " + (*(t_cube.get_center())).to_string() << endl;
  cout << endl;
  */

  // projeta cada um dos raios
  ofstream output;
  output.open("intersecoes.txt");
  BMP preview = BMP(panel_holes, panel_holes);
  float hole_width = panel_l/panel_holes;

  for(int i=0; i<panel_holes; i++)
  {
    for (int j=0; j<panel_holes; j++)
    {
      // gera o ponto da matriz em coordenadas de camera
      Point hole_point = Point(-panel_l/2 + hole_width/2 + j*hole_width, panel_l/2 - hole_width/2 - i*hole_width, -panel_d);
      // converte o ponto para coordenadas de mundo
      hole_point = camera.matrixTimesPoint(camera.camera_to_world(), hole_point);
      Vector3 ray_direction = Vector3(&observer, &hole_point);

      output << "Raio (" << i << ", " << j << "):" << endl;
      Ray ray = Ray(observer, ray_direction);
      float t_int;
      int object = 0;
      float t_min = 0;

      // interseções com os cilindros
      if(ray.intersect(cylinder, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CILINDRO: " << intersection << endl;
        t_min = t_int;
        object = 1;
      }
      if(ray.intersect(cylinder2, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CILINDRO2: " << intersection << endl;
        t_min = t_int;
        object = 1;
      }

      // interseções com os cones
      if(ray.intersect(cone, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CONE: " << intersection << endl;
        if(object == 0 || (t_int < t_min))
        {
          t_min = t_int;
          object = 2;
        }
      }
      if(ray.intersect(cone2, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CONE2: " << intersection << endl;
        if(object == 0 || (t_int < t_min))
        {
          t_min = t_int;
          object = 2;
        }
      }

      // interseções com o cubo base
      if(ray.intersect(b_cube, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CUBO BASE: " << intersection << endl;
        if(object == 0 || (t_int < t_min))
        {
          t_min = t_int;
          object = 3;
        }
      }

      // interseções com o cubo médio
      if(ray.intersect(m_cube, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CUBO MEDIO: " << intersection << endl;
        if(object == 0 || (t_int < t_min))
        {
          t_min = t_int;
          object = 3;
        }
      }

      // interseções com o cubo topo
      if(ray.intersect(t_cube, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CUBO TOPO: " << intersection << endl;
        if(object == 0 || (t_int < t_min))
        {
          t_min = t_int;
          object = 3;
        }
      }

      // colore o bitmap
      switch(object)
      {
        case 1: // cilindro
          preview.set_pixel(j, i, 69, 34, 0);
          break;
        case 2: // cone
          preview.set_pixel(j, i, 86, 125, 48);
          break;
        case 3: // algum cubo
          preview.set_pixel(j, i, 86, 48, 125);
          break;
        default: // sem intersecoes
          preview.set_pixel(j, i, 153, 204, 255);
          break;
      }
      output << endl;
    }
  }

  output.close();
  preview.write("preview.bmp");
  cout << "Resultados de preview escrito em \"preview.bmp\"" << endl;
  cout << "Resultados de interseções escritos em \"intersecoes.txt\"" << endl;
  return 0;
}

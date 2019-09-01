#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "Point.h"
#include "Vector3.h"
#include "Ray.h"
#include "Plane.h"
#include "Cylinder.h"
#include "Cone.h"
#include "AABB.h"

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

  cout << "Entre a distância da placa ao eixo y:" << endl;
  cin >> panel_d;

  cout << "Entre as coordenadas x, y e z do observador, separadas por espaços:" << endl;
  cin >> obx >> oby >> obz;
  // gera o ponto do observador
  Point observer = Point(obx, oby, obz);

  // calcula a largura dos furos
  float hole_width = panel_l/panel_holes;
  // matriz N*N
  Vector3 hole_matrix[panel_holes][panel_holes];
  // O ponto inicial está no menor x e no maior y
  float x_pos = -1*panel_l/2;
  float y_pos = panel_l/2;
  for(int i=0; i<panel_holes; i++)
  {
    for (int j=0; j<panel_holes; j++)
    {
      // sanity check: crio o ponto da matriz e só depois defino o vetor direção
      Point hole_point = Point(x_pos + hole_width/2, y_pos - hole_width/2, panel_d);
      hole_matrix[i][j] = Vector3(&observer, &hole_point);
      // cout << "Buraco (" << i << ", " << j << "): " << hole_matrix[i][j].to_string() << endl;
      x_pos += hole_width; // incrementa pos x
    }
    x_pos = -1*panel_l/2; // reseta posição x
    y_pos -= hole_width; // decrementa pos y
  }

  // gera o vetor normal a ser usado nos objetos
  Vector3 g_axis = Vector3(0, 1, 0);

  cout << "Gerando placa com " << panel_holes*panel_holes << " buracos e largura " << panel_l << "." << endl;
  float cx, cy, cz;
  cout << "Entre as coordenadas x, y e z do centro do cilindro, separadas por espaços:" << endl;
  cin >> cx >> cy >> cz;
  // cria o ponto do centro do cilindro
  Point cylinder_center = Point(cx, cy, cz);

  float ojb_height, obj_radius;
  cout << "Entre a altura e o raio do cilindro, separados por espaços:" << endl;
  cin >> ojb_height >> obj_radius;
  // gera o cilindro
  Cylinder cylinder = Cylinder(Point(cylinder_center.get_x() - obj_radius, cylinder_center.get_y(), cylinder_center.get_z()), cylinder_center, g_axis, ojb_height, obj_radius);

  cout << "Entre a altura e o raio do cone, separados por espaços:" << endl;
  cin >> ojb_height >> obj_radius;
  // copia as coordenadas para o ponto do centro
  float cox, coy, coz;
  cylinder_center.get_coordinates(&cox, &coy, &coz);
  float* cylinder_height = cylinder.get_height();
  // calcula cada coordenada com base na fórmula Pi = h + Ci*Ai 
  cox += (*cylinder_height) * g_axis.get_x();
  coy += (*cylinder_height) * g_axis.get_y();
  coz += (*cylinder_height) * g_axis.get_z();
  // gera o cone
  Cone cone = Cone(Point(cox, coy, coz), g_axis, ojb_height, obj_radius);

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
  for(int i=0; i<panel_holes; i++)
  {
    for (int j=0; j<panel_holes; j++)
    {
      output << "Raio (" << i << ", " << j << "):" << endl;
      Ray ray = Ray(observer, hole_matrix[i][j]);

      // interseções com o cilindro
      float t_int;
      if(ray.intersect(cylinder, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CILINDRO: " << intersection << endl;
      }

      // interseções com o cone
      if(ray.intersect(cone, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CONE: " << intersection << endl;
      }

      // interseções com o cubo base
      if(ray.intersect(b_cube, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CUBO BASE: " << intersection << endl;
      }

      // interseções com o cubo médio
      if(ray.intersect(m_cube, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CUBO MEDIO: " << intersection << endl;
      }

      // interseções com o cubo topo
      if(ray.intersect(t_cube, t_int))
      {
        Point intersection = ray.calc_point(t_int);
        output << " - CUBO TOPO: " << intersection << endl;
      }

      output << endl;
    }
  }
  cout << "Resultados de interseções escritos em \"intersecoes.txt\"" << endl;
  output.close();
  return 0;
}

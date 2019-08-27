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
  panel_d *= -1;
  cout << "Gerando placa com " << panel_holes*panel_holes << " buracos e largura " << panel_l << "." << endl;

  float hole_width = panel_l/panel_holes;
  // matriz (N/2) * N, uma vez que metade está escondida.
  Vector3 hole_matrix[panel_holes][panel_holes/2];
  // pego o maior tamanho em y para o ponto superior, e decremento no loop para os inferiores
  float y_pos = panel_l/2;
  for(int i=0; i<panel_holes/2; i++)
  {
    // decremento a posição y
    y_pos -= i*hole_width; 
    for (int j=0; j<panel_holes; j++)
    {
      hole_matrix[i][j] = Vector3((j*hole_width) + hole_width/2, y_pos - hole_width/2, panel_d);
      //cout << "Buraco (" << i << ", " << j << "): " << hole_matrix[i][j].to_string() << endl;
    }
  }

  cout << "Entre as coordenadas x, y e z do observador, separadas por espaços:" << endl;
  cin >> obx >> oby >> obz;
  while (obz > panel_d)
  {
    cout << "O observador não pode estar na frente da placa. Entre as coordenadas novamente:" << endl;
    cin >> obx >> oby >> obz;
  }
  // gera o ponto do observador
  Point observer = Point(obx, oby, obz);

  float cx, cy, cz;
  cout << "Entre as coordenadas x, y e z do centro do cilindro, separadas por espaços:" << endl;
  cin >> cx >> cy >> cz;
  // cria o ponto do centro do cilindro
  Point cylinder_center = Point(cx, cy, cz);

  cout << "Entre as coordenadas x, y e z do vetor direção do cilindro, separadas por espaços:" << endl;
  cin >> cx >> cy >> cz;
  // vetor direção do cilindro e do cone
  Vector3 cylinder_n = Vector3(cx, cy, cz);

  float ojb_height, obj_radius;
  cout << "Entre a altura e o raio do cilindro, separados por espaços:" << endl;
  cin >> ojb_height >> obj_radius;
  // gera o cilindro
  Cylinder cylinder = Cylinder(Point(cylinder_center.get_x() - obj_radius, cylinder_center.get_y(), cylinder_center.get_z()), cylinder_center, cylinder_n, ojb_height, obj_radius);

  cout << "Entre a altura e o raio do cone, separados por espaços:" << endl;
  cin >> ojb_height >> obj_radius;
  // calcula o vetor do centro do cilindro até o centro do cone
  Vector3 cy_center_cone_center = *(cylinder.get_axis()) * *(cylinder.get_height());
  // copia as coordenadas para o ponto do centro
  float cox, coy, coz;
  cy_center_cone_center.get_coordinates(&cox, &coy, &coz);
  // gera o cone
  Cone cone = Cone(Point(cox, coy, coz), *(cylinder.get_axis()), ojb_height, obj_radius);

  cout << "Entre as coordenadas x, y e z do centro do primeiro cubo, separados por espaços:" << endl;
  cout << "(os demais cubos serão calculados exatamente acima do primeiro)" << endl;
  cin >> cx >> cy >> cz;
  // cria o ponto do centro do cubo mais embaixo
  Point bcube_center = Point(cx, cy, cz);
  cout << "Entre as coordenadas x, y e z do vetor direção do primeiro cubo, separadas por espaços:" << endl;
  cin >> cx >> cy >> cz;
  // vetor direção dos cubos
  Vector3 cube_n = Vector3(cx, cy, cz);

  float cube_edge;
  cout << "Entre a largura da aresta do cubo:" << endl;
  cin >> cube_edge;
  // cria o cubo base
  AABB b_cube = AABB(bcube_center, cube_n, cube_edge);
  float bcx, bcy, bcz;
  bcube_center.get_coordinates(&bcx, &bcy, &bcz);
  float dx, dy, dz;
  cube_n.get_coordinates(&dx, &dy, &dz);
  // cria os demais cubos calculando o centro a partir do cubo base
  AABB m_cube = AABB(Point(bcx + (dx*cube_edge), bcy + (dy*cube_edge), bcz + (dz*cube_edge)), cube_n, cube_edge);
  AABB t_cube = AABB(Point(bcx + (2*dx*cube_edge), bcy + (2*dy*cube_edge), bcz + (2*dz*cube_edge)), cube_n, cube_edge);

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
  for(int i=0; i<panel_holes/2; i++)
  {
    for (int j=0; j<panel_holes; j++)
    {
      output << "Raio (" << i << ", " << j << "):" << endl;
      Ray temp = Ray(observer, hole_matrix[i][j]);

      // interseções com o cilindro
      vector<Point> intersections =  temp.intersect(cylinder);
      if(intersections.size() > 0)
        output << " - CILINDRO: " << intersections[0].to_string() << ", " << intersections[1].to_string() << endl;
      
      // interseções com o cone
      intersections =  temp.intersect(cone);
      if(intersections.size() > 0)
        output << " - CONE: " << intersections[0].to_string() << ", " << intersections[1].to_string() << endl;
      
      intersections =  temp.intersect(b_cube);
      // interseções com o cubo base
      if(intersections.size() > 0)
        output << " - CUBO BASE: " << intersections[0].to_string() << ", " << intersections[1].to_string() << endl;
      
      // interseções com o cubo médio
      intersections =  temp.intersect(m_cube);
      if(intersections.size() > 0)
        output << " - CUBO MEDIO: " << intersections[0].to_string() << ", " << intersections[1].to_string() << endl;
      
      // interseções com o cubo topo
      intersections =  temp.intersect(t_cube);
      if(intersections.size() > 0)
        output << " - CUBO TOPO: " << intersections[0].to_string() << ", " << intersections[1].to_string() << endl;
      
      output << endl;
    }
  }
  cout << "Resultados de interseções escritos em \"intersecoes.txt\"" << endl;
  output.close();
  return 0;
}

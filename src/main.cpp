#include <iostream>
#include <cmath>
#include <vector>
#include <limits>

#include <GL/freeglut.h>
#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_glut.h"
#include "./imgui/imgui_impl_opengl2.h"

#include "./geometry/Plane.hpp"
#include "./geometry/Sphere.hpp"
#include "./geometry/AABB.hpp"
#include "./geometry/Cone.hpp"
#include "./geometry/Cylinder.hpp"
#include "./geometry/Solid.hpp"
#include "./geometry/Object.hpp"

#include "./scene/Scene.hpp"

using namespace std;

int resolution = 500;
bool lightOn = true;
bool reRender = false;

Scene* scene;
Light* toggable_light;

Material* cone_color = new Material(RGB(0.33, 0.49, 0.18), RGB(0.2, 0.2, 0.2), RGB());
Material* cylinder_color = new Material(RGB(), RGB(0.27, 0.13, 0), RGB());
Material* cube_color = new Material(RGB(0.33, 0.18, 0.49), RGB(0.33, 0.18, 0.49), RGB());

GLubyte* PixelBuffer = new GLubyte[resolution * resolution * 3];

// Our state
static bool window_active = true;

float mat1[4] = { 0.33f, 0.49f, 0.18f, 1.0f };
float mat2[4] = { 0.27f, 0.13f, 0.0f, 1.0f };
float mat3[4] = { 0.33f, 0.18f, 0.49f, 1.0f };

void display_gui()
{
  ImGui::Begin("Materials", &window_active);

  if(ImGui::ColorEdit4("Cone", mat1))
  {
    cone_color->set_ambient(mat1[0], mat1[1], mat1[2]);
    scene->print(PixelBuffer);
    glutPostRedisplay();
  }
  if(ImGui::ColorEdit4("Cylinder", mat2))
  {
    cylinder_color->set_ambient(mat2[0], mat2[1], mat2[2]);
    scene->print(PixelBuffer);
    glutPostRedisplay();
  }
  if(ImGui::ColorEdit4("Cube", mat3))
  {
    cube_color->set_ambient(mat3[0], mat3[1], mat3[2]);
    scene->print(PixelBuffer);
    glutPostRedisplay();
  }

  ImGui::End();
}

void render()
{
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplGLUT_NewFrame();
  display_gui();
  ImGui::Render();
  //ImGuiIO& io = ImGui::GetIO();

  glClear(GL_COLOR_BUFFER_BIT);
  glDrawPixels(resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, PixelBuffer);

  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  glutSwapBuffers();
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  Point observer = Point(10, 4.5, 20);
  Point lookat = Point(10, 4.5, 5);
  Vector3 viewup = Vector3(10, 5.5, 20);

  Camera camera = Camera(observer, lookat, viewup);

  // definições de objetos
  Vector3 g_axis = Vector3(0, 1, 0);
  Point cylinder_center = Point(7, 0, 9);
  Point bcube_center = Point(10, 0, 5);

  float cylinder_radius = 0.5;
  float cylinder_height = 2;
  float cone_radius = 2;
  float cone_height = 8;
  float cube_edge = 3;

  // gera os objetos
  Cylinder cylinder = Cylinder(cylinder_center, g_axis, cylinder_height, cylinder_radius, cylinder_color);
  Cone cone = Cone(
    Point(
      cylinder_center.get_x() + cylinder_height * g_axis.get_x(),
      cylinder_center.get_y() + cylinder_height * g_axis.get_y(),
      cylinder_center.get_z() + cylinder_height * g_axis.get_z()
    ), g_axis, cone_height, cone_radius, cone_color);

  AABB b_cube = AABB(bcube_center, g_axis, cube_edge, cube_color);

  Object tree1 = Object(
    AABB(cylinder_center, g_axis, cone_radius*2, Vector3(0, cylinder_height+cone_height-cone_radius*2, 0)),
    vector<Solid*>{&cylinder, &cone}
  );

  Object tree2 = tree1.clone();
  tree2.translate(Vector3(6, 0, 0));

  Object building = Object(
    AABB(bcube_center, g_axis, cube_edge, Vector3()),
    vector<Solid*>{&b_cube}
  );

  vector<Object*> objects = {
    &tree1,
    &tree2,
    &building,
  };

  Light ambient_light = Light(0.5, 0.5, 0.5, Vector3(), AMBIENT);
  toggable_light = new Light(0.3, 0.3, 0.3, Vector3(15, 4.5, 15));
  vector<Light*> lights = {
    &ambient_light,
    toggable_light
  };

  scene = new Scene(resolution, camera, objects, lights);

  scene->print(PixelBuffer);

  // inicia GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(resolution, resolution);
  glutCreateWindow("Trabalho CG");

  glutDisplayFunc(render);

  // Inicia contexto imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();

  // Seta bindings com GLUT
  ImGui_ImplGLUT_Init();
  ImGui_ImplGLUT_InstallFuncs();
  ImGui_ImplOpenGL2_Init();

  glutMainLoop();

  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGLUT_Shutdown();
  ImGui::DestroyContext();

  return 0;
}

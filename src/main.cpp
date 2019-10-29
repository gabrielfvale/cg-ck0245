#include <iostream>
#include <stdlib.h>
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
float upscaling = 1.0f;
GLubyte* PixelBuffer;

static float observerf3[3] = { 10.0f, 4.5f, 10.0f };
static float lookatf3[3] = { 10.0f, 4.5f, 5.0f };
static float viewupf3[3] = { 10.0f, 5.5f, 10.0f };
Camera* camera = new Camera(observerf3, lookatf3, viewupf3);
Scene* scene;
Light* point_light = new Light(0.05, 0.05, 0.05, Vector3(10, 20, 5));
Light* remote_light = new Light(0.3, 0.3, 0.3, Vector3(-1, -1, 0), REMOTE);
bool pl_on = true; bool rl_on = true;

Material* cone_color = new Material(RGB(0.33, 0.49, 0.18), RGB(0.2, 0.2, 0.2), RGB(0.3, 0.3, 0.3));
Material* cylinder_color = new Material(RGB(0.27, 0.13, 0), RGB(0.27, 0.13, 0), RGB());
Material* cube_color = new Material(RGB(0.33, 0.18, 0.49), RGB(0.33, 0.18, 0.49), RGB());

float mat_ambient[3] = {0.0f, 0.0f, 0.0f};
float mat_diffuse[3] = {0.0f, 0.0f, 0.0f};
float mat_specular[3] = {0.0f, 0.0f, 0.0f};

Solid* picked_solid = NULL;
const char* object_selected = "No object selected";

void redraw()
{
  scene->print(PixelBuffer);
  glutPostRedisplay();
}

void display_gui()
{
  ImGuiColorEditFlags picker_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_Float;
  ImGuiWindowFlags window_flags = 0;
  //window_flags |= ImGuiWindowFlags_NoTitleBar;
  //window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoResize;
  //window_flags |= ImGuiWindowFlags_NoCollapse;
  window_flags |= ImGuiWindowFlags_NoBackground;
  window_flags |= ImGuiWindowFlags_NoSavedSettings;
  //window_flags |= ImGuiWindowFlags_NoDecoration;

  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
  ImGui::SetNextWindowSize(ImVec2(250, 300), ImGuiCond_Once);
  // Janela de propriedades do cenario
  ImGui::Begin("Scene", NULL, window_flags);
  // Propriedades de camera
  ImGui::Text("Camera");
  ImGui::InputFloat3("observer", observerf3);
  if(ImGui::IsItemDeactivatedAfterEdit())
  {
    camera->set_eye(observerf3);
    redraw();
  }
  ImGui::InputFloat3("lookat", lookatf3);
  if(ImGui::IsItemDeactivatedAfterEdit())
  {
    camera->set_lookat(lookatf3);
    redraw();
  }
  ImGui::InputFloat3("viewup", viewupf3);
  if(ImGui::IsItemDeactivatedAfterEdit())
  {
    camera->set_viewup(viewupf3);
    redraw();
  }
  // Interruptores de luzes
  ImGui::Text("Lights");
  if(ImGui::Checkbox("Remote light", &rl_on))
  {
    rl_on ? remote_light->set_intensity(0.3, 0.3, 0.3) : remote_light->set_intensity(0, 0, 0);
    redraw();
  }
  if(ImGui::Checkbox("Point light", &pl_on))
  {
    pl_on ? point_light->set_intensity(0.05, 0.05, 0.05) : point_light->set_intensity(0, 0, 0);
    redraw();
  }
  // Propriedades de materiais
  ImGui::BeginChild("Material");
  ImGui::Text("Materials");
  ImGui::Text(object_selected);
  if(picked_solid != NULL)
  {
    ImGui::ColorEdit3("Ambient", mat_ambient, picker_flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
      Material* material = picked_solid->get_material();
      material->set_ambient(mat_ambient);
      redraw();
    }
    ImGui::ColorEdit3("Diffuse", mat_diffuse, picker_flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
      Material* material = picked_solid->get_material();
      material->set_diffuse(mat_diffuse);
      redraw();
    }
    ImGui::ColorEdit3("Specular", mat_specular, picker_flags);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
      Material* material = picked_solid->get_material();
      material->set_specular(mat_specular);
      redraw();
    }
  }
  ImGui::EndChild();
  ImGui::End();
  // Object picking
  ImGuiIO& io = ImGui::GetIO();
  if(ImGui::IsMouseClicked(0) && !ImGui::IsMouseDragging())
  {
    // Manda um raio em MousePos.x, MousePos.y
    Intersection intersection;
    scene->castRay(io.MousePos.x/upscaling, io.MousePos.y/upscaling, intersection);
    if(intersection.index != -1)
    {
      cout << "Solid hit: " << *(intersection.solid_hit) << endl;
      Material* int_material = intersection.solid_hit->get_material();

      mat_ambient[0] = int_material->ambient.r;
      mat_ambient[1] = int_material->ambient.g;
      mat_ambient[2] = int_material->ambient.b;

      mat_diffuse[0] = int_material->diffuse.r;
      mat_diffuse[1] = int_material->diffuse.g;
      mat_diffuse[2] = int_material->diffuse.b;

      mat_specular[0] = int_material->specular.r;
      mat_specular[1] = int_material->specular.g;
      mat_specular[2] = int_material->specular.b;

      object_selected = intersection.solid_hit->name;
      picked_solid = intersection.solid_hit;
    }
  }
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
  glPixelZoom(upscaling, upscaling);

  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

  glutSwapBuffers();
  glutPostRedisplay();
}

int main(int argc, char *argv[])
{
  if(argc >= 2)
    resolution = atoi(argv[1]);
  if(argc == 3)
    upscaling = atof(argv[2]);
  PixelBuffer = new GLubyte[resolution * resolution * 3];
  // definições de objetos
  Vector3 g_axis = Vector3(0, 1, 0);
  float sphere_radius = 0.5;
  float cylinder_radius = 0.5;
  float cylinder_height = 2;
  float cone_radius = 2;
  float cone_height = 8;
  float cube_edge = 3;

  // gera os objetos
  Cylinder cylinder = Cylinder(Point(7, 0, 9), g_axis, cylinder_height, cylinder_radius, cylinder_color);
  Cone cone = Cone(
    Point(
      7 + cylinder_height * g_axis.get_x(),
      cylinder_height * g_axis.get_y(),
      9 + cylinder_height * g_axis.get_z()
    ), g_axis, cone_height, cone_radius, cone_color);

  AABB b_cube = AABB(Point(10, 0, 5), g_axis, cube_edge, cube_color);
  Sphere sp = Sphere(Point(10, 3.5, 5), sphere_radius, cone_color);
  cone_color->set_shine(10.0f);

  Object* tree1 = new Object(
    AABB(Point(7, 0, 9), g_axis, cone_radius*2, Vector3(0, cylinder_height+cone_height-cone_radius*2, 0)),
    vector<Solid*>{&cylinder, &cone}
  );
  Object* tree2 = tree1->clone();
  tree2->translate(Vector3(6, 0, 0));

  Object* cube = new Object(
    b_cube,
    vector<Solid*>{&b_cube}
  );
  Object* sphere = new Object(
    AABB(Point(10, 3, 5), Vector3(0, 1, 0), 1, Vector3()),
    vector<Solid*>{&sp}
  );
  /*
  Sphere sp2 = Sphere(Point(10, 1, 5), 1, cube_color);
  Object* sphere2 = new Object(
    AABB(Point(10, 2, 5), g_axis, 6, Vector3()),
    vector<Solid*>{&sp2}
  );
  */

  vector<Object*> objects = {
    tree1,
    tree2,
    cube,
    sphere
  };

  Light* ambient_light = new Light(0.5, 0.5, 0.5, Vector3(), AMBIENT);

  vector<Light*> lights = {
    ambient_light,
    point_light,
    remote_light
  };

  scene = new Scene(resolution, camera, objects, lights);

  scene->print(PixelBuffer);

  // inicia GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-resolution*upscaling)/2, (glutGet(GLUT_SCREEN_HEIGHT)-resolution*upscaling)/2);
  glutInitWindowSize(resolution*upscaling, resolution*upscaling);
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

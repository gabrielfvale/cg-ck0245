#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>

#include <GL/freeglut.h>
#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_glut.h"
#include "./imgui/imgui_impl_opengl2.h"

#include "./geometry/Plane.hpp"
#include "./geometry/Triangle.hpp"
#include "./geometry/Sphere.hpp"
#include "./geometry/AABB.hpp"
#include "./geometry/OBB.hpp"
#include "./geometry/Cone.hpp"
#include "./geometry/Cylinder.hpp"
#include "./geometry/Solid.hpp"
#include "./geometry/Object.hpp"

#include "./scene/Scene.hpp"

using namespace std;

int resolution = 500;
float upscaling = 1.0f;
GLubyte* PixelBuffer;

/* Origin */
static float observerf3[3] = { 0.0f, 160.0f, 500.0f };
static float lookatf3[3] = { 0.0f, 160.0f, 0.0f };
static float viewupf3[3] = { 0.0f, 161.0f, 500.0f };
/* Spot 
static float observerf3[3] = { 10.0f, 4.5f, 10.0f };
static float lookatf3[3] = { 10.0f, 4.5f, 5.0f };
static float viewupf3[3] = { 10.0f, 6.0f, 10.0f };
*/
Camera* camera = new Camera(observerf3, lookatf3, viewupf3);

float pl_intensity[3] = {0.3f, 0.3f, 0.3f};
float rl_intensity[3] = {0.3f, 0.3f, 0.3f};
float sp_intensity[3] = {0.3f, 0.3f, 0.3f};
Light* point_light = new Light(pl_intensity, Vector3(0, 250, 80));
Light* point_light2 = new Light(pl_intensity, Vector3(0, 250, 320));
Light* remote_light = new Light(rl_intensity, Vector3(-1, -1, 0), REMOTE);
Light* spot_light = new Light(sp_intensity, Point(0, 290/2, 180), Vector3(0, -1, 0), 15, 45, 5);

Scene* scene;
vector<Object*> objects;
/*
Material* mat_green = new Material(RGB(0.33, 0.49, 0.18), RGB(0.2, 0.2, 0.2), RGB(0.3, 0.3, 0.3), 10.0f);
Material* mat_brown = new Material(RGB(0.27, 0.13, 0), RGB(0.27, 0.13, 0), RGB());
Material* mat_purple = new Material(RGB(0.33, 0.18, 0.49), RGB(0.33, 0.18, 0.49), RGB());
Material* mat_gold = new Material(
  RGB(0.24725, 0.1995, 0.0745),
  RGB(0.75164, 0.60648, 0.22648),
  RGB(0.628281, 0.555802, 0.366065),
  128*0.4
);
Material* mat_jade = new Material(
  RGB(0.135, 0.2225, 0.1575),
  RGB(0.54, 0.89, 0.63),
  RGB(0.316228, 0.316228, 0.316228),
  12.8
);
Material* mat_ruby = new Material(
  RGB(0.1745, 0.01175, 0.01175),
  RGB(0.61424, 0.04136, 0.04136),
  RGB(0.727811, 0.626959, 0.626959),
  76.8
);
Material* mat_bronze = new Material(
  RGB(0.2125, 0.1275, 0.054),
  RGB(0.714, 0.4284, 0.18144),
  RGB(0.393548, 0.271906, 0.166721),
  25.6
);
*/
Material* mat_silver = new Material(
	RGB(0.23125, 0.23125, 0.23125),
  RGB(0.2775, 0.2775, 0.2775),
  RGB(0.773911, 0.773911, 0.773911),
  89.6
);
Material* mat_obsidian = new Material(
	RGB(0.05375, 0.05, 0.06625),
  RGB(0.18275, 0.17, 0.22525),
  RGB(0.332741, 0.328634, 0.346435),
  38.4
);

Material* mat_white_concrete = new Material(
  RGB(0.847058, 0.819607, 0.756862),
  RGB(0.854901, 0.843137, 0.815686),
  RGB()
);
Material* mat_beige_paint = new Material(
  RGB(0.819607, 0.776470, 0.698039),
  RGB(0.882352, 0.839215, 0.760784),
  RGB()
);
Material* mat_terrazo = new Material(
  RGB(0.490196, 0.454901, 0.435294),
  RGB(0.57254, 0.53725, 0.51764),
  RGB(0.3, 0.3, 0.3),
  38.0
);
Material* mat_darkwood = new Material(
  RGB(0.149019, 0.090196, 0.062745),
  RGB(0.01, 0.01, 0.01),
  RGB(0.1, 0.1, 0.1),
  38.0
);
Material* mat_old_plastic = new Material(
  RGB(0.772549, 0.721568, 0.549019),
  RGB(0.949019, 0.898039, 0.760784),
  RGB(0.5, 0.5, 0.5),
  32
);
Material* mat_black_plastic = new Material(
  RGB(0, 0, 0),
  RGB(0.01, 0.01, 0.01),
  RGB(0.5, 0.5, 0.5),
  32
);
Material* mat_white_plastic = new Material(
  RGB(0.933333, 0.925490, 0.878431),
  RGB(0.976470, 0.968627, 0.921568),
  RGB(0.5, 0.5, 0.5),
  32
);

float obj_ambient[3] = {0.0f, 0.0f, 0.0f};
float obj_diffuse[3] = {0.0f, 0.0f, 0.0f};
float obj_specular[3] = {0.0f, 0.0f, 0.0f};

float obj_translate[3] = {0.0f, 0.0f, 0.0f};
float obj_rangle = 0.0f;
float obj_raxis[3] = {0.0f, 0.0f, 0.0f};

Solid* picked_solid = NULL;
Object* picked_object = NULL;
const char* object_name = "No object selected";

void redraw()
{
  scene->print(PixelBuffer);
  glutPostRedisplay();
}

void display_gui()
{
  //ImGui::ShowDemoWindow();
  ImGuiStyle& style = ImGui::GetStyle();
  style.FrameRounding = 12.0f;
  //ImGui::StyleColorsLight();

  ImGuiColorEditFlags picker_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_Float;
  ImGuiWindowFlags window_flags = 0;
  //window_flags |= ImGuiWindowFlags_NoTitleBar;
  //window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_NoResize;
  //window_flags |= ImGuiWindowFlags_NoCollapse;
  //window_flags |= ImGuiWindowFlags_NoBackground;
  window_flags |= ImGuiWindowFlags_NoSavedSettings;
  //window_flags |= ImGuiWindowFlags_NoDecoration;
  window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
  //ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_Once);
  // Janela de propriedades do cenario
  ImGui::Begin("Scene", NULL, window_flags);
  /* Propriedades de camera */
  if(ImGui::CollapsingHeader("Camera"))
  {
    ImGui::InputFloat3("observer", observerf3);
    ImGui::InputFloat3("lookat", lookatf3);
    ImGui::InputFloat3("viewup", viewupf3);
    if(ImGui::Button("Update camera"))
    {
      camera->set_eye(observerf3);
      camera->set_lookat(lookatf3);
      camera->set_viewup(viewupf3);
      redraw();
    }
  }
  /* Configuração de luzes */
  if(ImGui::CollapsingHeader("Lights"))
  {
    if(ImGui::Checkbox("Remote light", remote_light->active()))
      redraw();
    ImGui::ColorEdit3("rl_rgb", rl_intensity);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
      remote_light->set_intensity(rl_intensity);
      redraw();
    }
    if(ImGui::Checkbox("Point light", point_light->active()))
      redraw();
    ImGui::ColorEdit3("pl_rgb", pl_intensity);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
      point_light->set_intensity(pl_intensity);
      redraw();
    }
    if(ImGui::Checkbox("Spot light", spot_light->active()))
      redraw();
    ImGui::ColorEdit3("sp_rgb", sp_intensity);
    if(ImGui::IsItemDeactivatedAfterEdit())
    {
      spot_light->set_intensity(sp_intensity);
      redraw();
    }
  }

  /* Object picking */
  if(ImGui::CollapsingHeader("Object Picking"))
  {
    ImGui::Text(object_name);
    if(picked_solid != NULL)
    {
      if(ImGui::Checkbox("Visible", picked_object->visible_ptr()))
        redraw();
      ImGui::ColorEdit3("Ambient", obj_ambient, picker_flags);
      if(ImGui::IsItemDeactivatedAfterEdit())
      {
        Material* material = picked_solid->get_material();
        material->set_ambient(obj_ambient);
        redraw();
      }
      ImGui::ColorEdit3("Diffuse", obj_diffuse, picker_flags);
      if(ImGui::IsItemDeactivatedAfterEdit())
      {
        Material* material = picked_solid->get_material();
        material->set_diffuse(obj_diffuse);
        redraw();
      }
      ImGui::ColorEdit3("Specular", obj_specular, picker_flags);
      if(ImGui::IsItemDeactivatedAfterEdit())
      {
        Material* material = picked_solid->get_material();
        material->set_specular(obj_specular);
        redraw();
      }
      ImGui::Text("Transformations");
      ImGui::InputFloat3("trl", obj_translate);
      if(ImGui::IsItemDeactivatedAfterEdit())
      {
        Vector3 trl_vector = Vector3(obj_translate);
        if(trl_vector.norm() != 0)
          picked_object->translate(trl_vector);
        redraw();
        obj_translate[0] = 0.0f;
        obj_translate[1] = 0.0f;
        obj_translate[2] = 0.0f;
      }
    }
  }
  if(ImGui::CollapsingHeader("Object selection"))
  {
    ImGui::BeginChild("selection", ImVec2(0, 100));
    for (unsigned i = 0; i < objects.size(); i++)
      ImGui::Selectable(objects[i]->name, objects[i]->visible_ptr());
    ImGui::EndChild();
    if(ImGui::Button("Update objects"))
      redraw();
  }
  ImGui::End();

  /* Object picking (Ray cast) */
  ImGuiIO& io = ImGui::GetIO();
  if(ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered())
  {
    // Manda um raio em MousePos.x, MousePos.y
    Intersection intersection;
    scene->castRay(io.MousePos.x/upscaling, io.MousePos.y/upscaling, intersection);
    if(intersection.index != -1)
    {
      cout <<  *(intersection.object_hit) << endl;
      Material* int_material = intersection.solid_hit->get_material();

      obj_ambient[0] = int_material->ambient.r;
      obj_ambient[1] = int_material->ambient.g;
      obj_ambient[2] = int_material->ambient.b;

      obj_diffuse[0] = int_material->diffuse.r;
      obj_diffuse[1] = int_material->diffuse.g;
      obj_diffuse[2] = int_material->diffuse.b;

      obj_specular[0] = int_material->specular.r;
      obj_specular[1] = int_material->specular.g;
      obj_specular[2] = int_material->specular.b;

      object_name = intersection.object_hit->name;
      picked_solid = intersection.solid_hit;
      picked_object = intersection.object_hit;
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
  Point origin;
  Vector3 g_axis = Vector3(0, 1, 0);

  /* CEOS */
  /* Paredes */
  float wall_height = 290.0;
  float wall_thickness = 12.5;
  /* Paredes frontais (alinhadas com eixo Z) */
  float front_wall_dim = 430;
    /* Parede de fundo */
  Point back_wall_start = Point(-front_wall_dim/2, 0, -wall_thickness);
  Point back_wall_end = Point(front_wall_dim/2, wall_height, 0);
  AABB* back_wall_rect = new AABB(back_wall_start, back_wall_end, mat_white_concrete);

  Object* back_wall = new Object(
    "Back wall",
    OBB(back_wall_start, back_wall_end),
    vector<Solid*>{back_wall_rect}
  );

  /* Paredes laterais (alinhadas com eixo X) */
  float side_wall_dim1 = 230.0;
  float side_wall_section = 22.0;
  float side_wall_dim2 = 108.0;
    /* Parede esquerda */
  Point left_wall_start = Point(back_wall_start.get_x(), 0, 0);
  Point left_wall_end = Point(back_wall_start.get_x()+wall_thickness, wall_height, side_wall_dim1+side_wall_section+side_wall_dim2);

  Point left_sect_start = Point(left_wall_end.get_x(), 0, side_wall_dim1);
  Point left_sect_end = Point(left_sect_start.get_x()+wall_thickness, wall_height, side_wall_dim1+side_wall_section);

  AABB* left_wall_rect = new AABB(left_wall_start, left_wall_end, mat_white_concrete);
  AABB* left_wall_sect = new AABB(left_sect_start, left_sect_end, mat_white_concrete);
  Object* left_wall = new Object(
    "Left wall",
    OBB(left_wall_start, Point(left_sect_end.get_x(), wall_height, left_wall_end.get_z())),
    vector<Solid*>{left_wall_rect, left_wall_sect}
  );

    /* Parede direita */
  Point right_wall_start = Point(back_wall_end.get_x()-wall_thickness, 0, 0);
  Point right_wall_end = Point(back_wall_end.get_x(), wall_height, side_wall_dim1+side_wall_section+side_wall_dim2);

  Point right_sect_start = Point(right_wall_end.get_x()-38, 0, side_wall_dim1);
  Point right_sect_end = Point(right_wall_end.get_x(), wall_height, side_wall_dim1+side_wall_section);

  AABB* right_wall_rect = new AABB(right_wall_start, right_wall_end, mat_white_concrete);
  AABB* right_wall_sect = new AABB(right_sect_start, right_sect_end, mat_white_concrete);
  Object* right_wall = new Object(
    "Right wall",
    OBB(Point(right_sect_start.get_x(), 0, 0), right_wall_end),
    vector<Solid*>{right_wall_rect, right_wall_sect}
  );

  /* Teto */
  Point ceiling_start = Point(back_wall_start.get_x(), back_wall_end.get_y(), 0);
  Point ceiling_end = Point(right_wall_end.get_x(), back_wall_end.get_y()+wall_thickness, right_wall_end.get_z());
  AABB* ceiling_rect = new AABB(ceiling_start, ceiling_end, mat_white_concrete);
  Object* ceiling = new Object(
    "Ceiling",
    OBB(ceiling_start, ceiling_end),
    vector<Solid*>{ceiling_rect}
  );

  /* Piso */
  Point floor_start = Point(back_wall_start.get_x(), -wall_thickness, 0);
  Point floor_end = Point(right_wall_end.get_x(), 0, right_wall_end.get_z());
  AABB* floor_rect = new AABB(floor_start, floor_end, mat_terrazo);
  Object* floor = new Object(
    "Floor",
    OBB(floor_start, floor_end),
    vector<Solid*>{floor_rect}
  );

  /* Rodapé */
  float footer_height = 6.0;
  float footer_thickness = 2.0;

  AABB* back_footer = new AABB(Point(back_wall_start.get_x(), 0, 0), Point(back_wall_end.get_x(), footer_height, footer_thickness), mat_darkwood);

  AABB* lfooter1 = new AABB(Point(left_wall_end.get_x(), 0, back_wall_end.get_z()+2*footer_height+70), Point(left_wall_end.get_x()+1.1*footer_thickness, footer_height, left_wall_end.get_z()), mat_darkwood);
  AABB* lfooter2 = new AABB(Point(left_sect_start.get_x(), 0, left_sect_start.get_z()-footer_thickness), Point(left_sect_end.get_x()+footer_thickness, footer_height, left_sect_end.get_z()+footer_thickness), mat_darkwood);

  AABB* rfooter1 = new AABB(Point(right_wall_start.get_x()-footer_thickness, 0, 0), Point(right_wall_start.get_x(), footer_height, right_wall_end.get_z()), mat_darkwood);
  AABB* rfooter2 = new AABB(Point(right_sect_start.get_x()-footer_thickness, 0, right_sect_start.get_z()-footer_thickness), Point(right_sect_end.get_x(), footer_height, right_sect_end.get_z()+footer_thickness), mat_darkwood);
  Object* footer = new Object(
    "Footer",
    OBB(Point(back_wall_start.get_x(), 0, 0), Point(back_wall_end.get_x(), 6, 360)),
    vector<Solid*>{back_footer, lfooter1, lfooter2, rfooter1, rfooter2}
  );

  /* Porta pro corredor */
  Point hall_d_start = Point(left_wall_end.get_x()-5, 0, back_wall_end.get_z()+footer_height);
  Point hall_d_end = Point(left_wall_end.get_x()+1, 210, back_wall_end.get_z()+footer_height+70);

  AABB* d_rect = new AABB(hall_d_start, hall_d_end, mat_beige_paint);
  AABB* d_contour_right = new AABB(Point(left_wall_end.get_x(), 0, back_wall_end.get_z()), Point(left_wall_end.get_x()+footer_thickness, hall_d_end.get_y(), back_wall_end.get_z()+footer_height), mat_darkwood);
  AABB* d_contour_top = new AABB(Point(left_wall_end.get_x(), hall_d_end.get_y(), back_wall_end.get_z()), Point(left_wall_end.get_x()+footer_thickness, hall_d_end.get_y()+footer_height, hall_d_end.get_z()+footer_height), mat_darkwood);
  AABB* d_contour_left = new AABB(Point(left_wall_end.get_x(), 0, hall_d_end.get_z()), Point(left_wall_end.get_x()+footer_thickness, hall_d_end.get_y(), hall_d_end.get_z()+footer_height), mat_darkwood);
  Object* hall_door = new Object(
    "Hall door",
    OBB(Point(left_wall_end.get_x(), 0, back_wall_end.get_z()), Point(left_wall_end.get_x()+footer_thickness, hall_d_end.get_y()+footer_height, hall_d_end.get_z()+footer_height)),
    vector<Solid*>{d_rect, d_contour_left, d_contour_right, d_contour_top}
  );

  /* AC Velho */
  Point old_acbox[2] = {Point(left_wall_end.get_x(), hall_d_end.get_y()+footer_height+10, hall_d_start.get_z()), Point(left_wall_end.get_x()+10, left_wall_end.get_y(), hall_d_end.get_z())};
  Point old_acbounds[2] = {Point(old_acbox[1].get_x()-10, old_acbox[0].get_y()+5, old_acbox[0].get_z()+5), Point(old_acbox[1].get_x()+5, old_acbox[0].get_y()+55, old_acbox[1].get_z()-5)};

  AABB* old_acbox_rect = new AABB(old_acbox[0], old_acbox[1], mat_white_concrete);
  AABB* old_ac_rect = new AABB(old_acbounds[0], old_acbounds[1], mat_old_plastic);
  Object* old_ac = new Object(
    "Old AC",
    OBB(old_acbox[0], Point(old_acbounds[1].get_x(), old_acbox[1].get_y(), old_acbox[1].get_z())),
    vector<Solid*>{old_acbox_rect, old_ac_rect}
  );

  /* AC Novo 
    Largura: 71,3cm
    Altura: 27cm
    Profundidade: 19,5cm
  */
  Object* new_ac = new Object("New AC", "./obj/AC/AC_chassi.obj", mat_white_plastic);
  new_ac->include("./obj/AC/AC_pholder.obj", mat_black_plastic);
  new_ac->include("./obj/AC/AC_plates.obj", mat_white_plastic);
  new_ac->translate(Vector3(left_wall_end.get_x()+90, wall_height-37, 0));


  /* Grade */
  AABB* gridBottom = new AABB(Point(right_wall_start.get_x(),130,right_wall_start.get_z()+245), Point(right_wall_start.get_x()+10, 137, right_wall_start.get_z()), mat_silver);
  AABB* gridMiddle1 = new AABB(Point(right_wall_start.get_x(),180,right_wall_start.get_z()+245), Point(right_wall_start.get_x()+10, 187, right_wall_start.get_z()), mat_silver);
  AABB* gridMiddle2 = new AABB(Point(right_wall_start.get_x(),230,right_wall_start.get_z()+245), Point(right_wall_start.get_x()+10, 237, right_wall_start.get_z()), mat_silver);
  AABB* gridTop = new AABB(Point(right_wall_start.get_x(),280,right_wall_start.get_z()+245), Point(right_wall_start.get_x()+10, 287, right_wall_start.get_z()), mat_silver);
  
  vector<Solid*> gridSolids = {gridBottom, gridMiddle1, gridMiddle2, gridTop};

  for(int i = 0; i < 21; i++){
    gridSolids.push_back(new Cylinder(Point(right_wall_start.get_x(),135,right_wall_start.get_z()+240-(i*11)), Vector3(0,1,0), 150.0f, 0.8f, mat_silver));
  }

  Object* grid = new Object(
    "Grid",
    OBB(Point(right_wall_start.get_x(),130,right_wall_start.get_z()+235), Point(right_wall_start.get_x()+10, 287, right_wall_start.get_z())),
    gridSolids
  );

  objects.push_back(hall_door);
  objects.push_back(old_ac);
  objects.push_back(new_ac);
  objects.push_back(floor);
  objects.push_back(footer);
  objects.push_back(left_wall);
  objects.push_back(ceiling);
  objects.push_back(back_wall);
  
  objects.push_back(right_wall);
  objects.push_back(grid);


  Light* ambient_light = new Light(RGB(0.5, 0.5, 0.5), Vector3(), AMBIENT);

  vector<Light*> lights = {
    ambient_light,
    point_light,
    point_light2,
    remote_light,
    spot_light
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

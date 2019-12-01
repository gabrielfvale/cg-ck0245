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
static float observerf3[3] = { 0.0f, 180.0f, 355.0f };
static float lookatf3[3] = { 0.0f, 160.0f, 0.0f };
static float viewupf3[3] = { 0.0f, 181.0f, 355.0f };

/* Top 
static float observerf3[3] = { 0.0f, 280.0f, 180.0f };
static float lookatf3[3] = { 0.0f, 0.0f, 180.0f };
static float viewupf3[3] = { 0.0f, 280.0f, 200.0f };
*/

Camera* camera = new Camera(observerf3, lookatf3, viewupf3);

/* Lights */
  /* Point lights */
float pl_intensity[4][3] = {
  {0.2f, 0.2f, 0.2f},
  {0.2f, 0.2f, 0.2f},
  {0.2f, 0.2f, 0.2f},
  {0.2f, 0.2f, 0.2f}
};
float pl_pos[4][3] = {
  {100, 270, 90},
  {-100, 270, 90},
  {100, 270, 260},
  {-100, 270, 260}
};
vector<Light*> point_lights = {
  new Light(pl_intensity[0], Vector3(pl_pos[0])),
  new Light(pl_intensity[1], Vector3(pl_pos[1])),
  new Light(pl_intensity[2], Vector3(pl_pos[2])),
  new Light(pl_intensity[3], Vector3(pl_pos[3]))
};
  /* Remote light */
float rl_intensity[3] = {0.3f, 0.3f, 0.3f};
float rl_dir[3] = {-1, -1, 0};
Light* remote_light = new Light(rl_intensity, Vector3(rl_dir), REMOTE);
  /* Spotlight */
float sp_intensity[3] = {0.3f, 0.3f, 0.3f};
float sp_pos[3] = {0, 145, 180};
float sp_dir[3] = {0, -1, 0};
float sp_angle = M_PI_4/3; float sp_falloff = M_PI_2/3; float sp_focus = 1;
Light* spot_light = new Light(sp_intensity, Point(sp_pos), Vector3(sp_dir), sp_angle, sp_falloff, sp_focus);

  /* Ambient light */
Light* ambient_light = new Light(RGB(0.5, 0.5, 0.5), Vector3(), AMBIENT);

vector<Light*> lights = {
  ambient_light,
  remote_light,
  spot_light
};
bool global_switch = true;
// Used for scene interaction
OBB* switch_top_bound = new OBB(Point(0, 0, -3.6), Point(0.6, 5.75, 3.6));
OBB* switch_bottom_bound = new OBB(Point(0, -5.75, -3.6), Point(0.6, 0, 3.6));

Scene* scene;
vector<Object*> objects;

/* Materials */
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
Material* mat_white_lamp = new Material(
  RGB(1, 1, 1),
  RGB(1, 1, 1),
  RGB(1, 1, 1)
);
Material* mat_steel = new Material(
	RGB(0.537354, 0.537354, 0.537354),
  RGB(0.772549, 0.772549, 0.772549),
  RGB(0.773911, 0.773911, 0.773911),
  32
);
Material* mat_marble = new Material(
  RGB(0.901960, 0.901960, 0.901960),
  RGB(0.949019, 0.949019, 0.949019),
  RGB(0.7, 0.7, 0.7),
  89.6
);
Material* mat_mdf = new Material(
  RGB(0.560784, 0.392156, 0.235294),
  RGB(0.901960, 0.811764, 0.662745),
  RGB(0.3, 0.3, 0.3),
  38
);
Material* mat_blue_chair = new Material(
  RGB(0.1921, 0.2588, 0.4274),
  RGB(0.2509, 0.4, 0.4980),
  RGB()
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
  if(ImGui::BeginTabBar("Main tab bar"))
  {
    /* Propriedades de camera */
    if(ImGui::BeginTabItem("Camera"))
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
      ImGui::EndTabItem();
    }

    /* Configuração de luzes */
    if(ImGui::BeginTabItem("Lights"))
    {
      if(ImGui::Checkbox("Toggle all", &global_switch))
      {
        for(auto light : lights)
          if(light->type() != AMBIENT)
            *(light->active()) = global_switch;

        redraw();
      }
      /* Point lights */
      if(ImGui::CollapsingHeader("Point lights"))
      {
        for(unsigned i = 0; i < point_lights.size(); i++)
        {
          string uid = "##pl_" + to_string(i+1);
          string i_light = "Point light " + uid;
          if(ImGui::TreeNode(i_light.c_str()))
          {
            string label_enabled = "Enabled" + uid;
            string label_position = "Position" + uid;
            string label_intensity = "Intensity" + uid;
            if(ImGui::Checkbox(label_enabled.c_str(), point_lights[i]->active()))
              redraw();
            ImGui::InputFloat3(label_position.c_str(), pl_pos[i]);
            if(ImGui::IsItemDeactivatedAfterEdit())
            {
              point_lights[i]->set_position(pl_pos[i]);
              redraw();
            }
            ImGui::ColorEdit3(label_intensity.c_str(), pl_intensity[i]);
            if(ImGui::IsItemDeactivatedAfterEdit())
            {
              point_lights[i]->set_intensity(pl_intensity[i]);
              redraw();
            }
            ImGui::TreePop();
          }
        }
      }
      /* Remote light */
      if(ImGui::CollapsingHeader("Remote light"))
      {
        if(ImGui::Checkbox("Enabled##rl_toggle", remote_light->active()))
          redraw();
        ImGui::InputFloat3("Direction##rl_dir", rl_dir, 3);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
          remote_light->set_position(Vector3(rl_dir));
          redraw();
        }
        ImGui::ColorEdit3("Intensity##rl_rgb", rl_intensity);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
          remote_light->set_intensity(rl_intensity);
          redraw();
        }
      }
      /* Spotlight */
      if(ImGui::CollapsingHeader("Spot light"))
      {
        if(ImGui::Checkbox("Enabled##sp_toggle", spot_light->active()))
          redraw();
        ImGui::SliderAngle("Angle", &sp_angle, 0.0f);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
          spot_light->set_spot(sp_pos, sp_angle, sp_falloff, sp_focus);
          redraw();
        }
        ImGui::SliderAngle("Falloff", &sp_falloff, (180/M_PI) * sp_angle);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
          spot_light->set_spot(sp_pos, sp_angle, sp_falloff, sp_focus);
          redraw();
        }
        ImGui::DragFloat("Focus", &sp_focus, 1, 1, 10);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
          spot_light->set_spot(sp_pos, sp_angle, sp_falloff, sp_focus);
          redraw();
        }
        ImGui::InputFloat3("Direction##sp_dir", sp_dir, 3);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
          spot_light->set_position(Vector3(sp_dir));
          redraw();
        }
        ImGui::InputFloat3("Position", sp_pos, 3);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
          spot_light->set_spot(sp_pos, sp_angle, sp_falloff, sp_focus);
          redraw();
        }
        ImGui::ColorEdit3("Intensity##sp_rgb ", sp_intensity);
        if(ImGui::IsItemDeactivatedAfterEdit())
        {
          spot_light->set_intensity(sp_intensity);
          redraw();
        }
      }
      ImGui::EndTabItem();
    }

    /* Object picking */
    if(ImGui::BeginTabItem("Object Picking"))
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
        ImGui::Text("Translate");
        ImGui::InputFloat3("##trl", obj_translate);
        ImGui::Text("Scale");
        ImGui::InputFloat3("###scl", obj_translate);
        ImGui::Text("Rotate");
        ImGui::DragFloat("Angle", &obj_rangle, 1, -360.0f, 360.0f);
        const char* axis_label[4] = {"X Axis", "Y Axis", "Z Axis", "Custom"};
        static const char* curr_axis = axis_label[0];
        float u_axis[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
        string custom_str = "Custom";
        if (ImGui::BeginCombo("Axis##combo", curr_axis))
        {
          for (int i = 0; i < 4; i++)
          {
            bool is_selected = (curr_axis == axis_label[i]);
            if (ImGui::Selectable(axis_label[i], is_selected))
            {
              curr_axis = axis_label[i];
              for(int k = 0; k < 3; k++)
                obj_raxis[k] = u_axis[i][k];
            }
          }
          ImGui::EndCombo();
        }
        if(custom_str.compare(curr_axis) == 0)
          ImGui::InputFloat3("##custom_axis", obj_raxis);
        if(ImGui::Button("Apply"))
        {
          // Translate
          Vector3 trl_vector = Vector3(obj_translate);
          if(trl_vector.norm() != 0)
            picked_object->translate(trl_vector);
          // Rotate
          Vector3 rot_axis = Vector3(obj_raxis);
          if(obj_rangle != 0 && rot_axis.norm() != 0)
          {
            float rad_angle = obj_rangle * (M_PI/180);
            picked_object->rotate(rad_angle, rot_axis);
          }
          redraw();
        }
        ImGui::SameLine();
        if(ImGui::Button("Reset"))
        {
          obj_rangle = 0.0f;
          for(int i = 0; i < 3; i++)
          {
            obj_translate[i] = 0.0f;
            obj_raxis[i] = 0.0f;
          }
        }
      }
      ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("Object selection"))
    {
      ImGui::BeginChild("selection", ImVec2(0, 100));
      for (unsigned i = 0; i < objects.size(); i++)
        ImGui::Selectable(objects[i]->name, objects[i]->visible_ptr());
      ImGui::EndChild();
      if(ImGui::Button("Update objects"))
        redraw();
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
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
      string str_obj(object_name);
      if(str_obj.compare("Light switch_0") == 0)
      {
        cout << "Switching back lights" << endl;
        *(point_lights[0]->active()) = ! *(point_lights[0]->active());
        *(point_lights[1]->active()) = ! *(point_lights[1]->active());
        redraw();
      } else if (str_obj.compare("Light switch_1") == 0)
      {
        cout << "Switching front lights" << endl;
        *(point_lights[2]->active()) = ! *(point_lights[2]->active());
        *(point_lights[3]->active()) = ! *(point_lights[3]->active());
        redraw();
      }
      
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

  Point origin;

  for(auto point_light : point_lights)
    lights.push_back(point_light);

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
  Object* front_wall = back_wall->clone("Front wall");
  front_wall->translate(Vector3(0, 0, 360+wall_thickness));

  /* Paredes laterais (alinhadas com eixo X) */
  float side_wall_dim1 = 230.0;
  float side_wall_section = 22.0;
  float side_wall_dim2 = 108.0;
  float window_thickness = 2;
    /* Parede esquerda */
  Point left_wall_start = Point(back_wall_start.get_x(), 0, 0);
  Point left_wall_end = Point(back_wall_start.get_x()+wall_thickness, wall_height, side_wall_dim1+side_wall_section+side_wall_dim2);

  Point left_sect_start = Point(left_wall_end.get_x(), 0, side_wall_dim1);
  Point left_sect_end = Point(left_sect_start.get_x()+wall_thickness, wall_height, side_wall_dim1+side_wall_section);

  AABB* left_wall_rect1 = new AABB(left_wall_start, Point(left_wall_end.get_x(), left_wall_end.get_y()-60, left_wall_end.get_z()), mat_white_concrete);
  AABB* left_wall_rect2 = new AABB(Point(left_wall_start.get_x(), left_wall_end.get_y()-60, left_wall_start.get_z()), Point(left_wall_end.get_x(), left_wall_end.get_y(), left_wall_start.get_z()+112), mat_white_concrete);
  AABB* left_wall_rect3 = new AABB(Point(left_wall_start.get_x(), left_wall_end.get_y()-60, left_sect_start.get_z()-9), Point(left_wall_end.get_x(), left_wall_end.get_y(), left_sect_start.get_z()), mat_white_concrete);
  AABB* left_wall_sect = new AABB(left_sect_start, left_sect_end, mat_white_concrete);
  Object* left_wall = new Object(
    "Left wall",
    OBB(left_wall_start, Point(left_sect_end.get_x(), wall_height, left_wall_end.get_z())),
    vector<Solid*>{left_wall_rect1, left_wall_rect2, left_wall_rect3, left_wall_sect}
  );

  /* Janelas da esquerda */
  Object* left_nwindow = new Object(
    "Left near window",
    OBB(Point(-window_thickness, 0, -54), Point(3, 60, 54)),
    vector<Solid*>{
      new AABB(Point(-window_thickness, 0, -54), Point(0, 5, 54), mat_steel), //bottom
      new AABB(Point(-window_thickness, 55, -54), Point(0, 60, 54), mat_steel), //top
      new AABB(Point(-window_thickness, 0, 54), Point(0, 60, 54-5), mat_steel), //left
      new AABB(Point(-window_thickness, 0, -54), Point(0, 60, -54+5), mat_steel), //right
      new AABB(Point(-window_thickness, 0, -4.5), Point(-0.5, 60, 4.5), mat_steel), //middle1
      new AABB(Point(-window_thickness, 0, -2.5), Point(0, 60, 2.5), mat_steel), //middle2
      new AABB(Point(0, 1, (54/2)-1), Point(0.5, 4.5, (54/2)+1), mat_black_plastic), //handle1_1
      new AABB(Point(0.5, 2.5, (54/2)-10), Point(1, 4, (54/2)+1), mat_steel), //handle1_2
      new AABB(Point(0, 1, (-54/2)-1), Point(0.5, 4.5, (-54/2)+1), mat_black_plastic), //handle2_1
      new AABB(Point(0.5, 2.5, (-54/2)-10), Point(1, 4, (-54/2)+1), mat_steel), //handle2_2
    }
  );
  Object* left_fwindow = left_nwindow->clone();
  left_nwindow->translate(Vector3(left_wall_end.get_x(), left_wall_end.get_y()-60, left_wall_end.get_z()-54));
  left_fwindow->translate(Vector3(left_wall_end.get_x(), left_wall_end.get_y()-60, left_wall_end.get_z()-193));

    /* Parede direita */
  Point right_wall1_start = Point(back_wall_end.get_x()-wall_thickness, 0, 0);
  Point right_wall1_end = Point(back_wall_end.get_x(), wall_height-165, side_wall_dim1);

  Point right_sect_start = Point(right_wall1_end.get_x()-wall_thickness-40, 0, side_wall_dim1);
  Point right_sect_end = Point(right_wall1_end.get_x(), wall_height, side_wall_dim1+side_wall_section);

  Point right_wall2_start = Point(right_wall1_start.get_x(), 0, right_sect_end.get_z());
  Point right_wall2_end = Point(back_wall_end.get_x(), wall_height, right_sect_end.get_z()+side_wall_dim2);

  AABB* right_wall1_rect = new AABB(right_wall1_start, right_wall1_end, mat_white_concrete);
  AABB* right_wall_r1_rect = new AABB(Point(right_wall1_start.get_x(), 275, 0), Point(back_wall_end.get_x(), wall_height, side_wall_dim1), mat_white_concrete);
  AABB* right_wall_r2_rect = new AABB(Point(right_wall1_start.get_x(), 125, 0), Point(back_wall_end.get_x(), wall_height, 5), mat_white_concrete);
  AABB* right_wall_r3_rect = new AABB(Point(right_wall1_start.get_x(), 125, side_wall_dim1-5), Point(back_wall_end.get_x(), wall_height, side_wall_dim1), mat_white_concrete);

  AABB* right_wall_sect = new AABB(right_sect_start, right_sect_end, mat_white_concrete);
  AABB* right_wall2_rect = new AABB(right_wall2_start, right_wall2_end, mat_white_concrete);

  Object* right_wall = new Object(
    "Right wall",
    OBB(Point(right_sect_start.get_x(), 0, 0), right_wall2_end),
    vector<Solid*>{right_wall1_rect, right_wall_r1_rect, right_wall_r2_rect, right_wall_r3_rect, right_wall_sect, right_wall2_rect}
  );

  /* Janela direita */
  AABB* window_f_cube = new AABB(Point(right_wall1_start.get_x(), wall_height-165, 5), Point(right_wall1_start.get_x()+window_thickness, wall_height-15, 10), mat_steel);
  AABB* window_ff_cube = new AABB(Point(right_wall1_start.get_x(), wall_height-165, 75), Point(right_wall1_start.get_x()+window_thickness, wall_height-15, 80), mat_steel);

  AABB* window_n_cube = new AABB(Point(right_wall1_start.get_x(), wall_height-165, side_wall_dim1-10), Point(right_wall1_start.get_x()+window_thickness, wall_height-15, side_wall_dim1-5), mat_steel);
  AABB* window_nn_cube = new AABB(Point(right_wall1_start.get_x(), wall_height-165, side_wall_dim1-75), Point(right_wall1_start.get_x()+window_thickness, wall_height-15, side_wall_dim1-70), mat_steel);

  AABB* window_t_cube = new AABB(Point(right_wall1_start.get_x(), wall_height-165, 5), Point(right_wall1_start.get_x()+window_thickness, wall_height-160, side_wall_dim1-5), mat_steel);
  AABB* window_b_cube = new AABB(Point(right_wall1_start.get_x(), wall_height-20, 5), Point(right_wall1_start.get_x()+window_thickness, wall_height-15, side_wall_dim1-5), mat_steel);
  AABB* window_m_cube = new AABB(Point(right_wall1_start.get_x(), wall_height-70, 5), Point(right_wall1_start.get_x()+window_thickness, wall_height-65, side_wall_dim1-5), mat_steel);
  Object* window = new Object(
    "Window frame",
    OBB(Point(right_wall1_start.get_x(), wall_height-165, 5), Point(right_wall1_start.get_x()+window_thickness, wall_height-15, side_wall_dim1-5)),
    vector<Solid*>{window_t_cube, window_b_cube, window_m_cube, window_f_cube, window_ff_cube, window_n_cube, window_nn_cube}
  );

  /* Teto */
  Point ceiling_start = Point(back_wall_start.get_x(), back_wall_end.get_y(), 0);
  Point ceiling_end = Point(right_wall2_end.get_x(), back_wall_end.get_y()+wall_thickness, right_wall2_end.get_z());
  AABB* ceiling_rect = new AABB(ceiling_start, ceiling_end, mat_white_concrete);
  Object* ceiling = new Object(
    "Ceiling",
    OBB(ceiling_start, ceiling_end),
    vector<Solid*>{ceiling_rect}
  );

  /* Piso */
  Point floor_start = Point(back_wall_start.get_x(), -wall_thickness, 0);
  Point floor_end = Point(right_wall2_end.get_x(), 0, right_wall2_end.get_z());
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

  AABB* lfooter0 = new AABB(Point(left_wall_end.get_x(), 0, back_wall_end.get_z()), Point(left_wall_end.get_x()+1.1*footer_thickness, footer_height, back_wall_end.get_z()+6), mat_darkwood);
  AABB* lfooter1 = new AABB(Point(left_wall_end.get_x(), 0, back_wall_end.get_z()+2*footer_height+86), Point(left_wall_end.get_x()+1.1*footer_thickness, footer_height, left_wall_end.get_z()), mat_darkwood);
  AABB* lfooter2 = new AABB(Point(left_sect_start.get_x(), 0, left_sect_start.get_z()-footer_thickness), Point(left_sect_end.get_x()+footer_thickness, footer_height, left_sect_end.get_z()+footer_thickness), mat_darkwood);

  AABB* rfooter1 = new AABB(Point(right_wall1_start.get_x()-footer_thickness, 0, 0), Point(right_wall1_start.get_x(), footer_height, right_wall2_end.get_z()), mat_darkwood);
  AABB* rfooter2 = new AABB(Point(right_sect_start.get_x()-footer_thickness, 0, right_sect_start.get_z()-footer_thickness), Point(right_sect_end.get_x(), footer_height, right_sect_end.get_z()+footer_thickness), mat_darkwood);
  Object* footer = new Object(
    "Footer",
    OBB(Point(back_wall_start.get_x(), 0, 0), Point(back_wall_end.get_x(), 6, 360)),
    vector<Solid*>{back_footer, lfooter0, lfooter1, lfooter2, rfooter1, rfooter2}
  );

  /* Porta pro corredor */
  Point hall_d_start = Point(left_wall_end.get_x()-5, 0, back_wall_end.get_z()+footer_height+6);
  Point hall_d_end = Point(left_wall_end.get_x()+1, 210, back_wall_end.get_z()+footer_height+86);

  AABB* d_rect = new AABB(hall_d_start, hall_d_end, mat_beige_paint);
  AABB* d_contour_right = new AABB(Point(left_wall_end.get_x(), 0, back_wall_end.get_z()+6), Point(left_wall_end.get_x()+footer_thickness, hall_d_end.get_y(), back_wall_end.get_z()+footer_height+6), mat_darkwood);
  AABB* d_contour_top = new AABB(Point(left_wall_end.get_x(), hall_d_end.get_y(), back_wall_end.get_z()+6), Point(left_wall_end.get_x()+footer_thickness, hall_d_end.get_y()+footer_height, hall_d_end.get_z()+footer_height), mat_darkwood);
  AABB* d_contour_left = new AABB(Point(left_wall_end.get_x(), 0, hall_d_end.get_z()), Point(left_wall_end.get_x()+footer_thickness, hall_d_end.get_y(), hall_d_end.get_z()+footer_height), mat_darkwood);

  AABB* handlebox = new AABB(Point(hall_d_end.get_x(), 80, hall_d_end.get_z()-2), Point(hall_d_end.get_x()+0.5, 97, hall_d_end.get_z()-6), mat_steel);
  Cylinder* handlecyl1 = new Cylinder(Point(hall_d_end.get_x()+0.5, 88.5, hall_d_end.get_z()-4), Vector3(1, 0, 0), 3, 1, mat_steel);
  Sphere* handlecurve = new Sphere(Point(hall_d_end.get_x()+3.5, 88.5, hall_d_end.get_z()-4), 1, mat_steel);
  Cylinder* handlecyl2 = new Cylinder(Point(hall_d_end.get_x()+3.5, 88.5, hall_d_end.get_z()-4), Vector3(0, 0, -1), 10, 1, mat_steel);

  Object* hall_door = new Object(
    "Hall door",
    OBB(Point(left_wall_end.get_x(), 0, back_wall_end.get_z()), Point(left_wall_end.get_x()+4, hall_d_end.get_y()+footer_height, hall_d_end.get_z()+footer_height)),
    vector<Solid*>{d_rect, d_contour_left, d_contour_right, d_contour_top, handlebox, handlecyl1, handlecurve, handlecyl2}
  );

  /* Interruptor */
  AABB* switchbox_rect = new AABB(Point(0, -5.75, -3.6), Point(0.6, 5.75, 3.6), mat_white_plastic);
  AABB* switch_t_toggle = new AABB(Point(0.6-0.25, 0.3, -1.75), Point(0.6+0.25, 1.3, 1.75), mat_old_plastic);
  AABB* switch_b_toggle = new AABB(Point(0.6-0.25, -1.3, -1.75), Point(0.6+0.25, -0.3, 1.75), mat_old_plastic);
  Object* light_switch_t = new Object(
    "Light switch_0",
    OBB(Point(0, 0, -3.6), Point(0.6+0.25, 5.75, 3.6)),
    vector<Solid*>{switchbox_rect, switch_t_toggle},
    mat_white_plastic
  );
  Object* light_switch_b = new Object(
    "Light switch_1",
    OBB(Point(0, -5.75, -3.6), Point(0.6+0.25, 0, 3.6)),
    vector<Solid*>{switchbox_rect, switch_b_toggle},
    mat_white_plastic
  );
  Vector3 switch_trl = Vector3(left_wall_end.get_x(), 105, hall_d_end.get_z()+footer_thickness+11.5);
  light_switch_t->translate(switch_trl); light_switch_b->translate(switch_trl);

  /* AC Velho */
  Point old_acbox[2] = {Point(left_wall_end.get_x(), hall_d_end.get_y()+footer_height+10, hall_d_start.get_z()), Point(left_wall_end.get_x()+10, ceiling_end.get_y(), hall_d_end.get_z())};
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
  float grid_offset = right_wall1_start.get_x() + window_thickness + 10;
  float s_height = 2;
  AABB* gridBottom = new AABB(Point(grid_offset,120,right_wall1_start.get_z()+245), Point(right_wall1_start.get_x()+10, 128+s_height, right_wall1_start.get_z()), mat_silver);
  AABB* gridMiddle1 = new AABB(Point(grid_offset,170,right_wall1_start.get_z()+245), Point(right_wall1_start.get_x()+10, 170+s_height, right_wall1_start.get_z()), mat_silver);
  AABB* gridMiddle2 = new AABB(Point(grid_offset,240,right_wall1_start.get_z()+245), Point(right_wall1_start.get_x()+10, 240+s_height, right_wall1_start.get_z()), mat_silver);
  AABB* gridTop = new AABB(Point(grid_offset,280,right_wall1_start.get_z()+245), Point(right_wall1_start.get_x()+10, 280+s_height, right_wall1_start.get_z()), mat_silver);

  vector<Solid*> gridSolids = {gridBottom, gridMiddle1, gridMiddle2, gridTop};

  for(int i = 0; i < 21; i++){
    gridSolids.push_back(new Cylinder(Point(grid_offset,110,right_wall1_start.get_z()+220-(i*11)), Vector3(0,1,0), 165.0f, 0.8f, mat_silver));
  }

  Object* grid = new Object(
    "Grid",
    OBB(Point(grid_offset,110,right_wall1_start.get_z()+235), Point(grid_offset+10, 285, right_wall1_start.get_z())),
    gridSolids
  );

  /* Armário */
  float lockerWidth = 108;
  float lockerHeight = 108;
  float lockerDepth = 40;
  
  AABB* locker_base = new AABB(Point(-lockerDepth/2+15, 0, -lockerWidth/2), Point(lockerDepth/2, 10, lockerWidth/2), mat_beige_paint);
  AABB* locker_back = new AABB(Point(lockerDepth/2-2, 10, -lockerWidth/2), Point(lockerDepth/2, 10+lockerHeight, lockerWidth/2), mat_beige_paint);
  AABB* locker_left = new AABB(Point(-lockerDepth/2,10,-lockerWidth/2), Point(lockerDepth/2,8+lockerHeight,-lockerWidth/2+2), mat_beige_paint);
  AABB* locker_right = new AABB(Point(-lockerDepth/2,10,lockerWidth/2-2), Point(lockerDepth/2,8+lockerHeight,lockerWidth/2), mat_beige_paint);
  AABB* locker_bottom = new AABB(Point(-lockerDepth/2, 10, -lockerWidth/2), Point(lockerDepth/2, 12, lockerWidth/2), mat_beige_paint);
  AABB* locker_middle = new AABB(Point(-lockerDepth/2+5, 10+lockerHeight/2, -lockerWidth/2), Point(lockerDepth/2, 12+lockerHeight/2, lockerWidth/2), mat_beige_paint);
  AABB* locker_top = new AABB(Point(-lockerDepth/2, 8+lockerHeight, -lockerWidth/2), Point(lockerDepth/2, 10+lockerHeight, lockerWidth/2), mat_beige_paint);
  AABB* locker_slider_left = new AABB(Point(-lockerDepth/2+3, 10, -lockerWidth/2+2), Point(-lockerDepth/2, 10+lockerHeight, 0), mat_old_plastic);
  AABB* locker_slider_right = new AABB(Point(-lockerDepth/2+3, 10, 0), Point(-lockerDepth/2+5, 10+lockerHeight, lockerWidth/2-2), mat_beige_paint);
  AABB* locker_holder_left = new AABB(Point(-lockerDepth/2-.5,7+lockerHeight/2,-lockerWidth/2+10), Point(-lockerDepth/2,12+lockerHeight/2,-lockerWidth/2+15), mat_silver);
  AABB* locker_holder_right = new AABB(Point(-lockerDepth/2+2.5,7+lockerHeight/2,lockerWidth/2-10), Point(-lockerDepth/2+3,12+lockerHeight/2,lockerWidth/2-15), mat_silver);

  Object* locker = new Object(
    "Locker",
    OBB(Point(-lockerDepth/2, 0, -lockerWidth/2), Point(lockerDepth/2, 10+lockerHeight, lockerWidth/2)),
    vector<Solid*> {locker_base, locker_back, locker_left, locker_right, locker_bottom, locker_middle, locker_top, locker_slider_left, locker_slider_right, locker_holder_left, locker_holder_right}
  );
  locker->translate(Vector3(right_wall1_end.get_x()-lockerDepth+8,0,306));

  /* Objetos em cima do armário */
    /* Porta retrato */
  Object* pic_frame = new Object(
    "Picture frame",
    OBB(Point(-1, 0, -6), Point(1, 20, 6)),
    vector<Solid*>{
      new AABB(Point(-0.9, 2, -4), Point(0.9, 13, 4), mat_white_plastic), // center
      new AABB(Point(-1, 0, -6), Point(1, 2, 6), mat_mdf), // bottom
      new AABB(Point(-1, 13, -6), Point(1, 15, 6), mat_mdf), // top
      new AABB(Point(-1, 0, -6), Point(1, 15, -4), mat_mdf), // left
      new AABB(Point(-1, 0, 4), Point(1, 15, 6), mat_mdf), // right
    }
  );
  pic_frame->translate(Vector3(180, 118, 270));
  pic_frame->rotate(15 * (M_PI/180), Vector3(0, 1, -1));

    /* Garrafa BTG */
  Object* btg_bottle = new Object(
    "BTG Bottle",
    OBB(Point(-3.5, 0, -3.5), Point(3.5, 22, 3.5)),
    vector<Solid*>{
      new Cylinder(Point(), Vector3(), 18.5, 3, mat_white_plastic),
      new Cylinder(Point(0, 8, 0), Vector3(), 10.5, 3.2, mat_white_plastic),
      new Cylinder(Point(0, 10, 0), Vector3(), 8.5, 3.4, mat_white_plastic),
      new Cylinder(Point(0, 12, 0), Vector3(), 6.5, 3.5, mat_white_plastic),
      new Cylinder(Point(0, 14, 0), Vector3(), 4.5, 3.5, mat_white_plastic),
      new Cylinder(Point(0, 18.5, 0), Vector3(), 0.5, 3, mat_white_plastic),
      new Cylinder(Point(0, 19, 0), Vector3(), 3, 3, mat_blue_chair)
    }
  );
  btg_bottle->translate(Vector3(190, 118, 298));
  Object* btg_bottle2 = btg_bottle->clone();
  btg_bottle2->translate(Vector3(0, 0, -7.5));
  Object* btg_bottle3 = btg_bottle2->clone();
  btg_bottle3->translate(Vector3(0, 0, -7.5));

    /* Copo vermelho */
  Object* red_cup = new Object(
    "Red cup",
    OBB(Point(-3, 0, -3), Point(3, 15, 3)),
    vector<Solid*>{
      new Cylinder(Point(), Vector3(), 15, 3, new Material(
        RGB(0.4588, 0.1803, 0.1960),
        RGB(0.6117, 0.1686, 0.2470),
        RGB(0.5, 0.5, 0.5),
        32
      ))
    }
  );
  red_cup->translate(Vector3(190, 118, 306));

    /* Copo turquesa */
  Object* turquoise_cup = new Object(
    "Turquoise cup",
    OBB(Point(-3.5, 0, -3.5), Point(3.5, 12, 3.5)),
    vector<Solid*>{
      new Cone(Point(), Vector3(), 40, 3.5, new Material(
        RGB(0.0666, 0.6078, 0.6196),
        RGB(0.0627, 0.6274, 0.6274),
        RGB()
      ))
    }
  );
  turquoise_cup->translate(Vector3(190, 118, 306+6.5));

    /* Copo branco */
  Object* white_cup = new Object(
    "White cup",
    OBB(Point(-3.5, 0, -3.5), Point(3.5, 15, 3.5)),
    vector<Solid*>{
      new Cone(Point(0, 15, 0), Vector3(0, -1, 0), 70, 3.5, new Material(
        RGB(0.9294, 0.9058, 0.8470),
        RGB(1, 0.9803, 0.9294),
        RGB()
      ))
    }
  );
  white_cup->translate(Vector3(190, 118, 327));

    /* Apagador */
  Object* board_eraser = new Object(
    "Eraser",
    OBB(Point(-7.5, 0, -3), Point(7.5, 4, 3)),
    vector<Solid*>{
      new AABB(Point(-6.5, 0, -2), Point(6.5, 3, 2), mat_black_plastic),
      new AABB(Point(-7, 2.5, -2.5), Point(7, 3, 2.5), mat_black_plastic),
      new AABB(Point(-6.5, 3, -2), Point(6.5, 3.5, 2), mat_black_plastic),
    }
  );
  board_eraser->translate(Vector3(176, 118, 340));
  board_eraser->rotate(7 * (M_PI/180), Vector3(0, 1, 0));

    /* Caneca */
  Object* white_mug = new Object(
    "White mug",
    OBB(Point(-3.5, 0, -3.5), Point(3.5, 12, 6.5)),
    vector<Solid*>{
      new Cylinder(Point(), Vector3(), 12, 3.5, mat_white_plastic),
      new AABB(Point(-0.5, 9, 3.5), Point(0.5, 9.5, 6), mat_white_plastic), // handle_t
      new AABB(Point(-0.5, 3, 3.5), Point(0.5, 3.5, 6), mat_white_plastic), // handle_b
      new AABB(Point(-0.5, 2.5, 6), Point(0.5, 10, 6.5), mat_white_plastic), // handle_m
    }
  );
  white_mug->translate(Vector3(178, 118, 332));
  white_mug->rotate(-M_PI_4, Vector3(0, 1, 0));

  /* Mesa central */
  float t_support = 3;
  float table_height = 80;
  Point table_mid_start = Point(-t_support/2, table_height/2, -60);
  Point table_mid_end = Point(t_support/2, table_height, 60);
  AABB* table_mid_rect = new AABB(table_mid_start, table_mid_end, mat_mdf);
  AABB* table_supf = new AABB(Point(-t_support/2-0.5, 0, -60-t_support), Point(t_support/2+0.5, table_height, -60), mat_black_plastic);
  AABB* table_supfh = new AABB(Point(-110/2, 0, -60-t_support), Point(110/2, 4, -60), mat_black_plastic);
  AABB* table_supn = new AABB(Point(-t_support/2-0.5, 0, 60), Point(t_support/2+0.5, table_height, 60+t_support), mat_black_plastic);
  AABB* table_supnh = new AABB(Point(-110/2, 0, 60), Point(110/2, 4, 60+t_support), mat_black_plastic);
  Object* table = new Object(
    "Table",
    OBB(Point(-110/2, 0, -60-t_support), Point(110/2, table_height, 60+t_support)),
    vector<Solid*>{table_supf, table_supfh, table_mid_rect, table_supn, table_supnh}
  );
  table->include("./obj/Table_top.obj", mat_mdf);
  table->translate(Vector3(0, 0, 180));

  /* Lâmpadas
   Comprimento da lampada: 120cm
   Raio da lampada: 2cm
   Comprimento da caixa: 130cm
   Largura da caixa: 12cm
  */
  float lampLength = 120.0f;
  float lampRadius = 2.5f;
  float lampBoxLength = 130.0f;
  float lampBoxWidth = 12.0f;
  float lampBoxHeight = 5.0f;
  float capRadius = 2.0f;
  float capLength = 2.0f;

  AABB* lampBox = new AABB(Point(lampBoxWidth/2, 0, lampBoxLength/2), Point(-lampBoxWidth/2, -lampBoxHeight, -lampBoxLength/2), mat_steel);
  Cylinder* lamp1 = new Cylinder(Point(lampBoxWidth/4,-lampBoxHeight-lampRadius-0.2, -lampBoxLength/2 + (lampBoxLength-lampLength)/2), Vector3(0,0,1), lampLength, lampRadius, mat_white_lamp);
  Cylinder* lamp2 = new Cylinder(Point(-lampBoxWidth/4,-lampBoxHeight-lampRadius-0.2, -lampBoxLength/2 + (lampBoxLength-lampLength)/2), Vector3(0,0,1), lampLength, lampRadius, mat_white_lamp);
  Cylinder* cap11 = new Cylinder(Point(lampBoxWidth/4,-lampBoxHeight-lampRadius-0.2, -lampBoxLength/2 - capLength + (lampBoxLength-lampLength)/2) , Vector3(0,0,1), capLength, capRadius, mat_silver);
  Cylinder* cap12 = new Cylinder(Point(lampBoxWidth/4,-lampBoxHeight-lampRadius-0.2, lampLength -lampBoxLength/2 + (lampBoxLength-lampLength)/2) , Vector3(0,0,1), capLength, capRadius, mat_silver);
  Cylinder* cap21 = new Cylinder(Point(-lampBoxWidth/4,-lampBoxHeight-lampRadius-0.2, -lampBoxLength/2 - capLength + (lampBoxLength-lampLength)/2) , Vector3(0,0,1), capLength, capRadius, mat_silver);
  Cylinder* cap22 = new Cylinder(Point(-lampBoxWidth/4,-lampBoxHeight-lampRadius-0.2, lampLength -lampBoxLength/2 + (lampBoxLength-lampLength)/2) , Vector3(0,0,1), capLength, capRadius, mat_silver);

  Object* lampObj = new Object(
    "Lamp Backg R",
    OBB(Point(Point(lampBoxWidth/2, 0, lampBoxLength/2)) ,Point(-lampBoxWidth/2, -lampBoxHeight - 2*lampRadius, -lampBoxLength/2)),
    vector<Solid*>{lampBox, lamp1, cap11, cap12, lamp2, cap21, cap22}
  );
  lampObj->translate(Vector3(100,290,back_wall_end.get_z()+90));

  Object* lampObj2 = lampObj->clone("Lamp Backg L");
  lampObj2->translate(Vector3(-200, 0 ,0));

  Object* lampObj3 = lampObj->clone("Lamp Foreg R");
  lampObj3->translate(Vector3(0, 0 , 170));

  Object* lampObj4 = lampObj3->clone("Lamp Foreg L");
  lampObj4->translate(Vector3(-200, 0 , 0));

  /* Quadro */
  float board_x_start = right_wall2_end.get_x()-80;
  float board_x_end = right_wall2_end.get_x()-280;
  float board_y_start = right_wall2_end.get_y()-70;
  float board_y_end = right_wall2_end.get_y()-215;
  float board_z_start = right_wall2_end.get_z()-2;
  float board_z_end = right_wall2_end.get_z();
  
  AABB* board = new AABB(
    Point(board_x_start,board_y_start,board_z_start),
    Point(board_x_end,board_y_end,board_z_end),
    mat_white_plastic
  );

  AABB* left_board_border = new AABB(
    Point(board_x_start,board_y_start,board_z_end),
    Point(board_x_start-6,board_y_end,board_z_end-4),
    mat_steel);
  AABB* right_board_border = new AABB(
    Point(board_x_end,board_y_start,board_z_end),
    Point(board_x_end+6,board_y_end,board_z_end-4),
    mat_steel);
  AABB* top_board_border = new AABB(
    Point(board_x_start,board_y_start,board_z_end),
    Point(board_x_end, board_y_start-6,board_z_end-4),
    mat_steel);
  AABB* bottom_board_border = new AABB(
    Point(board_x_start,board_y_end,board_z_end),
    Point(board_x_end, board_y_end+6,board_z_end-4),
    mat_steel); 

  Object* boardObj = new Object(
    "Board",
    OBB(Point(board_x_start,board_y_start,board_z_start),
    Point(board_x_end,board_y_end,board_z_end)),
    vector<Solid*>{board,left_board_border,right_board_border,top_board_border,bottom_board_border}
  );

  Object* chair1 = new Object(
    "Chair 1",
    "./obj/Chair/Chair_Frame.obj",
    mat_black_plastic
  );
  chair1->include("./obj/Chair/Chair_Back.obj", mat_blue_chair);
  chair1->include("./obj/Chair/Chair_Seat.obj", mat_blue_chair);
  Object* chair2 = chair1->clone("Chair 2");
  Object* chair3 = chair1->clone("Chair 3");
  Object* chair4 = chair1->clone("Chair 4");
  Object* chair5 = chair1->clone("Chair 5");

  chair1->translate(Vector3(0, 0, 70));

  chair2->translate(Vector3(-80, 0, 160));
  chair2->rotate(80 * (M_PI/180), Vector3(0, 1, 0));

  chair3->translate(Vector3(-70, 0, 220));
  chair3->rotate(107 * (M_PI/180), Vector3(0, 1, 0));

  chair4->translate(Vector3(60, 0, 160));
  chair4->rotate(-M_PI_2, Vector3(0, 1, 0));

  chair5->translate(Vector3(70, 0, 220));
  chair5->rotate(-M_PI_2, Vector3(0, 1, 0));

  /* Monitores */
  float baseRadius = 10.0f;
  float monitorNeckRadius = 2.0f;
  float monitorNeckHeight = 6.0f;
  float monitorScreenHeight = 23.0f;
  float monitorScreenWidth = 40.0f;
  float monitorBorderWidth = 2.0f;
  //float monitorScreenDepth = 5.0f;

  AABB* monitorBase = new AABB(Point(-baseRadius,0,-baseRadius),Point(baseRadius,1,baseRadius), mat_black_plastic);
  Cone* monitorNeck = new Cone(Point(0,0,0), Vector3(0,1,0), monitorNeckHeight+2, monitorNeckRadius,  mat_black_plastic);
  AABB* monitorBorderBottom = new AABB(Point(-monitorBorderWidth/2,monitorNeckHeight, -monitorScreenWidth/2),Point(monitorBorderWidth/2,monitorNeckHeight+monitorBorderWidth, monitorScreenWidth/2), mat_black_plastic);
  AABB* monitorBorderTop = new AABB(Point(-monitorBorderWidth/2,monitorNeckHeight+monitorBorderWidth+monitorScreenHeight, -monitorScreenWidth/2),Point(monitorBorderWidth/2,monitorNeckHeight+2*monitorBorderWidth+monitorScreenHeight, monitorScreenWidth/2), mat_black_plastic);
  AABB* monitorBorderLeft = new AABB(Point(-monitorBorderWidth/2,monitorNeckHeight+monitorBorderWidth, -monitorScreenWidth/2),Point(monitorBorderWidth/2,monitorNeckHeight+monitorBorderWidth+monitorScreenHeight, -monitorScreenWidth/2+monitorBorderWidth), mat_black_plastic);
  AABB* monitorBorderRight = new AABB(Point(-monitorBorderWidth/2,monitorNeckHeight+monitorBorderWidth, monitorScreenWidth/2),Point(monitorBorderWidth/2,monitorNeckHeight+monitorBorderWidth+monitorScreenHeight, monitorScreenWidth/2-monitorBorderWidth), mat_black_plastic);
  AABB* monitorScreen = new AABB(Point(-0.5,monitorNeckHeight+monitorBorderWidth, -monitorScreenWidth/2),Point(monitorBorderWidth/2-0.5,monitorNeckHeight+monitorBorderWidth+monitorScreenHeight, monitorScreenWidth/2-monitorBorderWidth), mat_white_lamp);
  AABB* monitorBack = new AABB(Point(monitorBorderWidth/2-1,monitorNeckHeight+monitorBorderWidth, -monitorScreenWidth/2),Point(monitorBorderWidth/2,monitorNeckHeight+monitorBorderWidth+monitorScreenHeight, monitorScreenWidth/2-monitorBorderWidth),  mat_black_plastic);

  Object* monitor = new Object(
    "Monitor",
    OBB(Point(-baseRadius,0, -monitorScreenWidth/2),Point(baseRadius,monitorNeckHeight+2*monitorBorderWidth+monitorScreenHeight, monitorScreenWidth/2)),
    vector<Solid*>{monitorBase, monitorNeck, monitorBorderBottom, monitorBorderTop, monitorBorderLeft, monitorBorderRight, monitorScreen, monitorBack}
  );
  monitor->translate(Vector3(0,table_height+3,200));
  Object* monitor2 = monitor->clone();
  monitor2->translate(Vector3(0,0,-50));

  /* Teclados */
  AABB* tecladoBase = new AABB(Point(-6.0f,0.0f,-21.5f), Point(6.0f,2.0f,21.5f), mat_white_plastic);
  AABB* tecladoTecla1 = new AABB(Point(5.5f,2.0f,-20.5f), Point(4.5f,2.5f,-19.5f), mat_old_plastic);
  AABB* tecladoTecla2= new AABB(Point(5.5f,2.0f,-18.5f), Point(4.5f,2.5f,5.0f), mat_old_plastic);
  AABB* tecladoTecla3= new AABB(Point(5.5f,2.0f, 6.0f), Point(0.0f,2.5f, 11.5f), mat_old_plastic);
  AABB* tecladoTecla4= new AABB(Point(-5.5,2.0f, -20.5), Point(3.5,2.5f, 5.0f), mat_old_plastic);
  AABB* tecladoTecla5= new AABB(Point(-5.5,2.0f, 6.0f), Point(-4.0f,2.5f, 11.0f), mat_old_plastic);
  AABB* tecladoTecla6= new AABB(Point(-4.0,2.0f, 7.6f), Point(-2.5f,2.5f, 9.2f), mat_old_plastic);
  AABB* tecladoTecla7= new AABB(Point(-5.5f,2.0f, 12.5f), Point(3.5f,2.5f, 20.5f), mat_old_plastic);

  Object* teclado = new Object(
    "Teclado",
    OBB(Point(-6.0f,0.0f,-21.5f), Point(6.0f,2.5f,21.5f)),
    vector<Solid*>{tecladoBase, tecladoTecla1, tecladoTecla2, tecladoTecla3, tecladoTecla4, tecladoTecla5, tecladoTecla6, tecladoTecla7}
  );

  teclado->translate(Vector3(-30,table_height+3,200));
  Object* teclado2 = teclado->clone();
  teclado2->translate(Vector3(0,0,-50));

  /* Paredes frontais */
  objects.push_back(back_wall);
  objects.push_back(new_ac);
  objects.push_back(front_wall);
  objects.push_back(boardObj);
  /* Parede direita */
  objects.push_back(right_wall);
  objects.push_back(window);
  objects.push_back(grid);
  objects.push_back(locker);
  /* Objetos em cima do armário */
  objects.push_back(pic_frame);
  objects.push_back(btg_bottle);
  objects.push_back(btg_bottle2);
  objects.push_back(btg_bottle3);
  objects.push_back(red_cup);
  objects.push_back(turquoise_cup);
  objects.push_back(white_cup);
  objects.push_back(white_mug);
  objects.push_back(board_eraser);
  /* Parede esquerda*/
  objects.push_back(left_wall);
  objects.push_back(left_nwindow);
  objects.push_back(left_fwindow);
  objects.push_back(hall_door);
  objects.push_back(old_ac);
  objects.push_back(light_switch_t);
  objects.push_back(light_switch_b); 
  /* Piso */
  objects.push_back(floor);
  objects.push_back(footer);
  objects.push_back(table);
  objects.push_back(chair1);
  objects.push_back(chair2);
  objects.push_back(chair3);
  objects.push_back(chair4);
  objects.push_back(chair5);
  /* Monitores e Teclados */
  objects.push_back(monitor);
  objects.push_back(monitor2);
  objects.push_back(teclado);
  objects.push_back(teclado2);
  /* Teto */
  objects.push_back(ceiling);
  objects.push_back(lampObj);
  objects.push_back(lampObj2);
  objects.push_back(lampObj3);
  objects.push_back(lampObj4);

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

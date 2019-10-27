#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <limits>
#include <GL/freeglut.h>

#include "RGB.hpp"
#include "Camera.hpp"
#include "../geometry/Object.hpp"
#include "Light.hpp"
#include "Intersection.hpp"

using std::vector;
using std::numeric_limits;

class Scene
{
  private:
    int resolution;
    float width;
    float distance;
    Camera camera;
    vector<Object*> objects;
    vector<Light*> lights;
    void set_pixel(GLubyte* pixels, int x, int y, RGB rgb);
  public:
    Scene(int resolution, Camera camera, vector<Object*> objects, vector<Light*> lights, float w = 6, float d = 3);
    bool trace(Ray& ray, Intersection& intersection);
    void castRay(int x, int y, RGB& color);
    void print(GLubyte* pixels);
};

#endif

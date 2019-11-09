#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

class Object; // foward declaration, avoiding dependency cycle

#include "../geometry/Solid.hpp"
#include "../geometry/Object.hpp"
#include "RGB.hpp"

class Intersection
{
  public:
    Object* object_hit; // object cluster
    Solid* solid_hit;
    float tint;
    int index;
    RGB color;
    Intersection()
    {
      object_hit = NULL;
      solid_hit = NULL;
      tint = 0;
      index = -1;
      color = RGB();
    }
};

#endif

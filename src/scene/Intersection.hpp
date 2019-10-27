#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "../geometry/Solid.hpp"
#include "RGB.hpp"

class Intersection
{
  public:
    Solid* solid_hit;
    float tint;
    int index;
    RGB color;
    Intersection()
    {
      solid_hit = NULL;
      tint = 0;
      index = -1;
      color = RGB();
    }
};

#endif

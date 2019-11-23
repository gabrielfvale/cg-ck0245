#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "../math/Point.hpp"
#include "RGB.hpp"

class Texture
{
  private:
    unsigned char *data;
    int w, h;
  public:
    Texture() {}
    Texture(unsigned char *pixels, int w, int h);
    RGB value(float u, float v, const Point& p);
};

#endif

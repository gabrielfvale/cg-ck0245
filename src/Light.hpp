#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "RGB.hpp"
#include "Point.hpp"

class Light
{
  private:
    RGB intensity_;
  public:
    Light();
    Light(RGB intensity);
    Light(float r, float g, float b);
    void set_intensity(RGB* new_intensity);
    void set_intensity(float r, float g, float b);
    RGB* get_intensity();
};

#endif

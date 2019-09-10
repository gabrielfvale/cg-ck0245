#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "RGB.hpp"

class Material
{
  private:
    RGB ambient_;
    RGB diffuse_;
    RGB specular_;
  public:
    Material();
    Material(RGB ambient, RGB diffuse, RGB specular);
    void set_ambient(float r, float g, float b);
    void set_diffuse(float r, float g, float b);
    void set_specular(float r, float g, float b);
};

#endif

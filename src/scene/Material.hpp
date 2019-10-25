#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "RGB.hpp"

class Material
{
  public:
    RGB ambient;
    RGB diffuse;
    RGB specular;
    float shine;
    Material();
    Material(RGB ambient, RGB diffuse, RGB specular);
    void set_ambient(float r, float g, float b);
    void set_diffuse(float r, float g, float b);
    void set_specular(float r, float g, float b);
    void set_shine(float s);
};

#endif

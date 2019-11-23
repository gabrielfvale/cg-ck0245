#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Texture.hpp"
#include "RGB.hpp"

class Material
{
  private:
    Texture* texture = NULL;
  public:
    RGB ambient;
    RGB diffuse;
    RGB specular;
    float shine;
    Material();
    Material(RGB ambient, RGB diffuse, RGB specular, float shine = 1);
    Material(RGB ambient, RGB specular, Texture* texture, float shine = 1);
    void set_ambient(float* rgb);
    void set_diffuse(float* rgb);
    void set_specular(float* rgb);
    void set_shine(float s);
    RGB lambertian(float u, float v, Point& p_int);
};

#endif

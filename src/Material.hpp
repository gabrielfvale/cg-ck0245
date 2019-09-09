#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Intensity.hpp"

class Material
{
  private:
    Intensity reflectivity_;
    Intensity specularity_;
  public:
    Material();
    Material(Intensity reflectivity, Intensity specularity);
    Material(float ref_r, float ref_g, float ref_b, float spec_r, float spec_g, float spec_b);
    void set_reflectivity(float r, float g, float b);
    void set_specularity(float r, float g, float b);
};

#endif

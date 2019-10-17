#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "RGB.hpp"
#include "Point.hpp"
#include "Vector3.hpp"

enum LightType { AMBIENT = 1, REMOTE = 2, POINT = 3 };

class Light
{
  private:
    RGB intensity_;
    Vector3 position_;
    LightType l_type;
  public:
    Light();
    Light(RGB intensity, Vector3 position, LightType light_type = POINT);
    Light(float r, float g, float b, Vector3 position, LightType light_type = POINT);
    void set_intensity(RGB new_intensity);
    void set_intensity(float r, float g, float b);
    void set_position(Vector3 position);
    RGB* get_intensity();
    Vector3* get_position();
    LightType type();
};

#endif

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "RGB.hpp"
#include "../math/Point.hpp"
#include "../math/Vector3.hpp"

enum LightType { AMBIENT = 1, REMOTE = 2, POINT = 3 };

class Light
{
  private:
    RGB intensity_;
    Vector3 position_;
    LightType l_type;
    bool active_;
  public:
    Light();
    Light(RGB intensity, Vector3 position, LightType light_type = POINT);
    Light(float* rgb, Vector3 position, LightType light_type = POINT);
    void set_intensity(RGB new_intensity);
    void set_intensity(float* rgb);
    void set_position(Vector3 position);
    bool* active();
    RGB* get_intensity();
    Vector3* get_position();
    LightType type();
};

#endif

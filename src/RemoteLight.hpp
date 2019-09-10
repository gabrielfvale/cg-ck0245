#ifndef REMOTE_LIGHT_HPP
#define REMOTE_LIGHT_HPP

#include "Light.hpp"
#include "Vector3.hpp"

class RemoteLight : public Light
{
  private:
    Vector3 direction_;
  public:
    RemoteLight();
    RemoteLight(RGB intensity, Vector3 direction);
    RemoteLight(float r, float g, float b, Vector3 direction);
    void set_direction(Vector3* direction);
    Vector3* get_direction();
};

#endif

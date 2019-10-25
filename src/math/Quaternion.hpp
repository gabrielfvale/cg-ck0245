#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include "Vector3.hpp"

class Quaternion
{
  private:
    Vector3 v_;
    float w_;
  public:
    Quaternion();
    Quaternion(Vector3 v, float w);
    Quaternion(float x, float y, float z, float w);
    Vector3* get_vec();
    float get_scalar();
    void set_vec(Vector3& v);
    void set_scalar(float w);
    void set(float x, float y, float z, float w);
    float length();
    Quaternion conjugate();
    Quaternion operator*(Quaternion& qr);
};

#endif

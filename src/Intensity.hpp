#ifndef INTENSITY_HPP
#define INTENSITY_HPP

class Intensity
{
  public:
    float r, g, b;
    Intensity();
    Intensity(float r, float g, float b);
    Intensity operator+(Intensity& intensity);
    Intensity operator*(float& value);
};

#endif

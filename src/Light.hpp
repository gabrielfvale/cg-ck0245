#ifndef LIGHT_HPP
#define LIGHT_HPP

struct Intensity
{
  float r;
  float g;
  float b;
};

class Light
{
  private:
    Intensity intensity_;
  public:
    Light();
    Light(Intensity intensity);
    Light(float r, float g, float b);
    void set_intensity(Intensity* new_intensity);
    void set_intensity(float r, float g, float b);
    Intensity* get_intensity();
};

#endif

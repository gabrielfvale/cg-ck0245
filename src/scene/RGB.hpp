#ifndef RGB_HPP
#define RGB_HPP

class RGB
{
  public:
    float r, g, b;
    RGB();
    RGB(float r, float g, float b);
    RGB operator+(RGB& intensity);
    RGB operator*(RGB& intensity);
    RGB operator*(float& value);
  private:
    void limit_range(float& value);
};

#endif

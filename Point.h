#ifndef POINT_H
#define POINT_H

class Point
{
  private:
    float x_, y_, z_;

  public:
    Point();
    Point(float x, float y, float z);
    float get_x();
    float get_y();
    float get_z();
    void get_coordinates(float* x, float* y, float* z);
    void set_coordinates(float x, float y, float z);
    float distance_from(Point* p);
};

#endif

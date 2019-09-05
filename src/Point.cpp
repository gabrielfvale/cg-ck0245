#include <cmath>
#include <iostream>
#include <string>

#include "Point.hpp"

using namespace std;

Point::Point() { x_ = 0; y_ = 0; z_ = 0; }
Point::Point(float x, float y, float z) { x_ = x; y_ = y; z_ = z; };

float Point::get_x() { return x_; };
float Point::get_y() { return y_; }
float Point::get_z() { return z_; }

void Point::get_coordinates(float* x, float* y, float* z)
{
  *x = x_;
  *y = y_;
  *z = z_; 
}
void Point::set_coordinates(float x, float y, float z) { x_ = x; y_ = y; z_ = z; }

float Point::distance_from(Point* p)
{
  float x1, y1, z1;
  (*p).get_coordinates(&x1, &y1, &z1);
  float sx, sy, sz;
  sx = pow(x_ - x1, 2);
  sy = pow(y_ - y1, 2);
  sz = pow(z_ - z1, 2);
  return sqrt(sx + sy + sz);
}
std::ostream& operator<<(std::ostream& stream, Point& point)
{
  stream << "(" << point.get_x() << ", " << point.get_y() << ", " << point.get_z() << ")";
  return stream;
}

#include <iostream>

#include "Point.h"
#include "Vector3.h"
#include "Ray.h"
#include "Plane.h"

using namespace std;

int main()
{
  Vector3 u(-1, -2, 3);
  Vector3 v(4, 0, -8);
  cout << "u: " + u.to_string() << endl;
  cout << "v: " + v.to_string() << endl;
  float dot_product = u.dot_product(&v);
  Vector3 cross_product = u.cross_product(&v);
  cout << "Dot product u . v: " << dot_product << endl;
  cout << "Cross product u x v: " << (u * v).to_string() << endl;

  Ray r = Ray(Point(2, 1, 1), Vector3(-1, 3, -2));
  Plane p = Plane(Point(1, 0, 1), Vector3(3, 1, 2));
  Point* t = r.intersect(p);
  cout << t->to_string() << endl;
  return 0;
}

#include <iostream>
#include <vector>

#include "Point.h"
#include "Vector3.h"
#include "Ray.h"
#include "Plane.h"
#include "Sphere.h"

using namespace std;

int main()
{
  Vector3 u(2.5, -1.25, 2);
  Vector3 v(0, 1, 0);
  cout << "u: " + u.to_string() << endl;
  cout << "v: " + v.to_string() << endl;
  float dot_product = u.dot_product(&v);
  Vector3 cross_product = u.cross_product(&v);
  cout << "Dot product u . v: " << dot_product << endl;
  cout << "Cross product u x v: " << (u * v).to_string() << endl;
  cout << endl;

  Ray r = Ray(Point(-2, 2, -1), u);
  Plane p = Plane(Point(1, 0, 1), v);
  Sphere s = Sphere(Point(2, 0, 0), Point(), 2);
  Point* ray_plane = r.intersect(p);
  std::vector<Point> ray_sphere = r.intersect(s);
  if(ray_plane) {
    cout << "Ray-Plane intersection:" << endl;
    cout << ray_plane->to_string() << endl;
    cout << endl;
  }
  if(ray_sphere.size() > 0) {
    cout << "Ray-Sphere intersections:" << endl;
    cout << ray_sphere[0].to_string() << endl;
    cout << ray_sphere[1].to_string() << endl;
    cout << endl;
  }
  return 0;
}

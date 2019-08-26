#include <iostream>
#include <vector>

#include "Point.h"
#include "Vector3.h"
#include "Ray.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Cone.h"
#include "AABB.h"
#include "OBB.h"

using namespace std;

int main()
{
  Vector3 u(2.5, -1.25, 2);
  Vector3 v(0, 1, 0);
  cout << "u: " + u.to_string() << endl;
  cout << "v: " + v.to_string() << endl;
  float dot_product = u.dot_product(&v);
  cout << "Dot product u . v: " << dot_product << endl;
  cout << "Cross product u x v: " << (u * v).to_string() << endl;
  cout << endl;
  // Example ray (certainty of intersectin)
  Ray ray = Ray(Point(-2, 2, -1), u);
  // Ray that intersects with some base
  // Ray y_shifted = Ray(Point(0.5, 2, 0.5), Vector3(0, 3, 0));
  // Easier ray for testing
  Ray inclined = Ray(Point(4, 0, 0), Vector3(-4, 5, 0));

  Plane plane = Plane(Point(1, 0, 1), v);
  Sphere sphere = Sphere(Point(2, 0, 0), Point(), 2);
  Cylinder cylinder = Cylinder(Point(2, 1, 0), Point(), Vector3(0, 1, 0), 2, 1);
  Cone cone = Cone(Point(), Vector3(0, 1, 0), 3, 1);
  AABB cube = AABB(Point(), Vector3(0, 0, 0), 4);
  // Intersections
  Point* ray_plane = ray.intersect(plane);
  std::vector<Point> ray_sphere = ray.intersect(sphere);
  std::vector<Point> ray_cylinder = ray.intersect(cylinder);
  std::vector<Point> ray_cone = ray.intersect(cone);
  std::vector<Point> ray_cube = inclined.intersect(cube);
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
  if(ray_cylinder.size() > 0) {
    cout << "Ray-Cylinder intersections:" << endl;
    cout << ray_cylinder[0].to_string() << endl;
    cout << ray_cylinder[1].to_string() << endl;
    cout << endl;
  }
  if(ray_cone.size() > 0) {
    cout << "Ray-Cone intersections:" << endl;
    cout << ray_cone[0].to_string() << endl;
    cout << ray_cone[1].to_string() << endl;
    cout << endl;
  }
  if(ray_cube.size() > 0) {
    cout << "Ray-Cube intersections:" << endl;
    cout << ray_cube[0].to_string() << endl;
    cout << ray_cube[1].to_string() << endl;
    cout << endl;
  }
  return 0;
}

#include "Camera.h"

Camera::Camera(Point eye, Point lookat, Vector3 up)
{
  eye_ = eye;
  lookat_ = lookat;
  up_ = up;
  cz_ = Vector3(&eye_, &lookat_);
  cz_.normalize();
  up_.normalize();

  cx_ = up_.cross_product(&cz_); // already normalized
  cy_ = cz_.cross_product(&cx_);
}

std::vector<std::vector<float>> Camera::lookat_matrix()
{
  float ex, ey, ez;
  eye_.get_coordinates(&ex, &ey, &ez);
  Vector3 eye_vector = Vector3(ex, ey, ez);
  std::vector<std::vector<float>> cameraToWorld(4, std::vector<float>(4));

  cameraToWorld[0][0] = cx_.get_x();
  cameraToWorld[0][1] = cx_.get_y();
  cameraToWorld[0][2] = cx_.get_y();
  cameraToWorld[0][3] = -1 * eye_vector.dot_product(&cx_);

  cameraToWorld[1][0] = cy_.get_x();
  cameraToWorld[1][1] = cy_.get_y();
  cameraToWorld[1][2] = cy_.get_y();
  cameraToWorld[1][3] = -1 * eye_vector.dot_product(&cy_);

  cameraToWorld[2][0] = cz_.get_x();
  cameraToWorld[2][1] = cz_.get_y();
  cameraToWorld[2][2] = cz_.get_y();
  cameraToWorld[2][3] = -1 * eye_vector.dot_product(&cz_);

  cameraToWorld[3][0] = 0;
  cameraToWorld[3][1] = 0;
  cameraToWorld[3][2] = 0;
  cameraToWorld[3][3] = 1;

  return cameraToWorld;
}

Point Camera::matrixTimesPoint(std::vector<std::vector<float>> cameraToWorld, Point& origin)
{
  float ox, oy, oz;
  float newx, newy, newz;
  origin.get_coordinates(&ox, &oy, &oz);
  newx = cameraToWorld[0][0] * ox + cameraToWorld[0][1] * oy + cameraToWorld[0][2] * oz;
  newy = cameraToWorld[1][0] * ox + cameraToWorld[1][1] * oy + cameraToWorld[1][2] * oz;
  newz = cameraToWorld[2][0] * ox + cameraToWorld[2][1] * oy + cameraToWorld[2][2] * oz;

  return Point(newx, newy, newz);
}

Vector3* Camera::x_axis() { return &cx_; }
Vector3* Camera::y_axis() { return &cy_; }
Vector3* Camera::z_axis() { return &cz_; }

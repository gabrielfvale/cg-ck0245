#include "Camera.hpp"

Camera::Camera()
{
  eye_ = Point(0, 0, 0);
  lookat_ = Point(0, 0, 1);
  up_ = Point(0, 1, 0);
}

Camera::Camera(Point eye, Point lookat, Point up)
{
  eye_ = eye;
  lookat_ = lookat;
  up_ = up;
  update();
}
Camera::Camera(float* eye, float* lookat, float* up)
{
  eye_ = Point(eye);
  lookat_ = Point(lookat);
  up_ = Point(up);
  update();
}

void Camera::update()
{
  Vector3 v = Vector3(&eye_, &up_);

  cz_ = Vector3(&lookat_, &eye_);
  cz_.normalize();
  cx_ = v.cross_product(&cz_);
  cx_.normalize();
  cy_ = cz_.cross_product(&cx_);
}

Matrix4 Camera::world_to_camera()
{
  float ex, ey, ez;
  eye_.get_coordinates(&ex, &ey, &ez);
  Vector3 eye_vector = Vector3(ex, ey, ez);
  Matrix4 matrix;

  matrix(0, 0) = cx_.get_x();
  matrix(0, 1) = cx_.get_y();
  matrix(0, 2) = cx_.get_z();
  matrix(0, 3) = -eye_vector.dot_product(&cx_);

  matrix(1, 0) = cy_.get_x();
  matrix(1, 1) = cy_.get_y();
  matrix(1, 2) = cy_.get_z();
  matrix(1, 3) = -eye_vector.dot_product(&cy_);

  matrix(2, 0) = cz_.get_x();
  matrix(2, 1) = cz_.get_y();
  matrix(2, 2) = cz_.get_z();
  matrix(2, 3) = -eye_vector.dot_product(&cz_);

  matrix(3, 0) = 0;
  matrix(3, 1) = 0;
  matrix(3, 2) = 0;
  matrix(3, 3) = 1;

  return matrix;
}

Matrix4 Camera::camera_to_world()
{
  Matrix4 matrix;

  matrix(0, 0) = cx_.get_x();
  matrix(1, 0) = cx_.get_y();
  matrix(2, 0) = cx_.get_z();
  matrix(3, 0) = 0;

  matrix(0, 1) = cy_.get_x();
  matrix(1, 1) = cy_.get_y();
  matrix(2, 1) = cy_.get_z();
  matrix(3, 1) = 0;

  matrix(0, 2) = cz_.get_x();
  matrix(1, 2) = cz_.get_y();
  matrix(2, 2) = cz_.get_z();
  matrix(3, 2) = 0;

  matrix(0, 3) = eye_.get_x();
  matrix(1, 3) = eye_.get_y();
  matrix(2, 3) = eye_.get_z();
  matrix(3, 3) = 1;

  return matrix;
}

Vector3* Camera::x_axis() { return &cx_; }
Vector3* Camera::y_axis() { return &cy_; }
Vector3* Camera::z_axis() { return &cz_; }
Point* Camera::get_eye() { return &eye_; }
void Camera::set_eye(float* coords)
{
  eye_.set_coordinates(coords[0], coords[1], coords[2]);
  update();
}
void Camera::set_lookat(float* coords)
{
  lookat_.set_coordinates(coords[0], coords[1], coords[2]);
  update();
}
void Camera::set_viewup(float* coords)
{
  up_.set_coordinates(coords[0], coords[1], coords[2]);
  update();
}

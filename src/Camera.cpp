#include "Camera.hpp"

Camera::Camera(Point eye, Point lookat, Vector3 up)
{
  eye_ = eye;
  lookat_ = lookat;
  up_ = up;
  cz_ = Vector3(&lookat_, &eye);

  Point up_point = Point(up_.get_x(), up_.get_y(), up_.get_z());
  Vector3 v = Vector3(&eye_, &up_point);
  cx_ = v.cross_product(&cz_);

  cx_.normalize();
  cz_.normalize();
  cy_ = cz_.cross_product(&cx_);
}

std::vector<std::vector<float>> Camera::world_to_camera()
{
  float ex, ey, ez;
  eye_.get_coordinates(&ex, &ey, &ez);
  Vector3 eye_vector = Vector3(ex, ey, ez);
  std::vector<std::vector<float>> matrix(4, std::vector<float>(4));

  matrix[0][0] = cx_.get_x();
  matrix[0][1] = cx_.get_y();
  matrix[0][2] = cx_.get_z();
  matrix[0][3] = -1 * eye_vector.dot_product(&cx_);

  matrix[1][0] = cy_.get_x();
  matrix[1][1] = cy_.get_y();
  matrix[1][2] = cy_.get_z();
  matrix[1][3] = -1 * eye_vector.dot_product(&cy_);

  matrix[2][0] = cz_.get_x();
  matrix[2][1] = cz_.get_y();
  matrix[2][2] = cz_.get_z();
  matrix[2][3] = -1 * eye_vector.dot_product(&cz_);

  matrix[3][0] = 0;
  matrix[3][1] = 0;
  matrix[3][2] = 0;
  matrix[3][3] = 1;

  return matrix;
}

std::vector<std::vector<float>> Camera::camera_to_world()
{
  std::vector<std::vector<float>> matrix(4, std::vector<float>(4));

  matrix[0][0] = cx_.get_x();
  matrix[1][0] = cx_.get_y();
  matrix[2][0] = cx_.get_z();
  matrix[3][0] = 0;

  matrix[0][1] = cy_.get_x();
  matrix[1][1] = cy_.get_y();
  matrix[2][1] = cy_.get_z();
  matrix[3][1] = 0;

  matrix[0][2] = cz_.get_x();
  matrix[1][2] = cz_.get_y();
  matrix[2][2] = cz_.get_z();
  matrix[3][2] = 0;

  matrix[0][3] = eye_.get_x();
  matrix[1][3] = eye_.get_y();
  matrix[2][3] = eye_.get_z();
  matrix[3][3] = 1;

  return matrix;
}

Point Camera::matrixTimesPoint(std::vector<std::vector<float>> transformation_matrix, Point& origin)
{
  float ox, oy, oz;
  float newx, newy, newz;
  origin.get_coordinates(&ox, &oy, &oz);
  newx = transformation_matrix[0][0] * ox + transformation_matrix[0][1] * oy + transformation_matrix[0][2] * oz + transformation_matrix[0][3];
  newy = transformation_matrix[1][0] * ox + transformation_matrix[1][1] * oy + transformation_matrix[1][2] * oz + transformation_matrix[1][3];
  newz = transformation_matrix[2][0] * ox + transformation_matrix[2][1] * oy + transformation_matrix[2][2] * oz + transformation_matrix[2][3];

  return Point(newx, newy, newz);
}

Vector3 Camera::matrixTimesVector(std::vector<std::vector<float>> transformation_matrix, Vector3& origin)
{
  float ox, oy, oz;
  float newx, newy, newz;
  origin.get_coordinates(&ox, &oy, &oz);
  newx = transformation_matrix[0][0] * ox + transformation_matrix[0][1] * oy + transformation_matrix[0][2] * oz;
  newy = transformation_matrix[1][0] * ox + transformation_matrix[1][1] * oy + transformation_matrix[1][2] * oz;
  newz = transformation_matrix[2][0] * ox + transformation_matrix[2][1] * oy + transformation_matrix[2][2] * oz;

  return Vector3(newx, newy, newz);
}

Vector3* Camera::x_axis() { return &cx_; }
Vector3* Camera::y_axis() { return &cy_; }
Vector3* Camera::z_axis() { return &cz_; }

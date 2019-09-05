#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>
#include "Vector3.hpp"

class Camera
{
  private:
    Point eye_;
    Point lookat_;
    Vector3 up_;
    Vector3 cx_, cy_, cz_;
  public:
    Camera(Point eye, Point lookat, Vector3 up);
    std::vector<std::vector<float>> camera_to_world();
    std::vector<std::vector<float>> world_to_camera();
    Point matrixTimesPoint(std::vector<std::vector<float>> transformation_matrix, Point& origin);
    Vector3 matrixTimesVector(std::vector<std::vector<float>> transformation_matrix, Vector3& origin);
    Vector3* x_axis();
    Vector3* y_axis();
    Vector3* z_axis();
};

#endif

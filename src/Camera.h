#ifndef CAMERA_H
#define CAMERA_H

#include <vector>
#include "Vector3.h"

class Camera
{
  private:
    Point eye_;
    Point lookat_;
    Vector3 up_;
    Vector3 cx_, cy_, cz_;
  public:
    Camera(Point eye, Point lookat, Vector3 up);
    std::vector<std::vector<float>> lookat_matrix();
    Point matrixTimesPoint(std::vector<std::vector<float>> cameraToWorld, Point& origin);
    Vector3* x_axis();
    Vector3* y_axis();
    Vector3* z_axis();
};

#endif

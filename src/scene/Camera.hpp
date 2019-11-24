#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>
#include "../math/Vector3.hpp"
#include "../math/Matrix4.hpp"

class Camera
{
  private:
    Point eye_;
    Point lookat_;
    Point up_;
    Vector3 cx_, cy_, cz_;
    void update();
  public:
    Camera();
    Camera(Point eye, Point lookat, Point up);
    Camera(float* eye, float* lookat, float* up);
    Matrix4 camera_to_world();
    Matrix4 world_to_camera();
    Vector3* x_axis();
    Vector3* y_axis();
    Vector3* z_axis();
    Point* get_eye();
    void set_eye(float* coords);
    void set_lookat(float* coords);
    void set_viewup(float* coords);
};

#endif

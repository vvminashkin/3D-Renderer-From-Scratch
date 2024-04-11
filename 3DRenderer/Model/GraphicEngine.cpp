#include "GraphicEngine.h"
#include "Eigen/src/Core/Matrix.h"
#include "Primitives.h"
#include "Screen.h"
#include "Utils.h"
#include "Object.h"
#include <algorithm>
#include <cmath>
#include <memory>

namespace kernel {
GraphicEngine::GraphicEngine(int width, int height)
    : width_(width),
      height_(height),
      current_screen_(width, height),
      update_port_([this]() -> std::reference_wrapper<const renderer::Screen> {
          return this->current_screen_;
      }),
      world_(width, height) {
    renderer::BasicObject object([](const renderer::Triangle&, Eigen::Vector3d) -> renderer::RGB {
        return {1, 0, 0};
    });
    Eigen::Matrix3d test_triangle;
    test_triangle << -0.61, -4.62, -9.54, 0.99, 6.23, -9.13, 1.73, 0.09, -4.32;
    object.AddTriangle(test_triangle);
    world_.AddObject(object);
    renderer::BasicObject object1([](const renderer::Triangle&, Eigen::Vector3d) -> renderer::RGB {
        return {0, 1, 0};
    });
    Eigen::Matrix3d test_triangle1;
    test_triangle1 << -0.13, 0.27, -3.15, 2.77, 1.39, -9.45, -2, -2, -12;
    object1.AddTriangle(test_triangle1);
    world_.AddObject(object1);
}

const renderer::Screen& GraphicEngine::GetCurrentScreen() {
    return current_screen_;
}

void GraphicEngine::Subscribe(observer::CObserver<const renderer::Screen>* obs) {
    update_port_.subscribe(obs);
}
void GraphicEngine::TestUpdateProjection() {
    std::unique_ptr<Screen> screen(renderer_.Draw(world_, width_, height_));
    current_screen_ = std::move(*screen.release());
    update_port_.notify();
}
void GraphicEngine::TiltCameraUp() {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Vector3d direction = camera_rotation * World::CameraHolder::kDefaultDirection;
    Vector3d normal = camera_rotation * World::CameraHolder::kDefaultNormal;
    Vector3d axis_of_rotation = direction.cross(normal);

    world_.SetCameraRotation(Quaterniond(camera_rotation.matrix() *
                                         Eigen::AngleAxisd(0.004, axis_of_rotation).matrix()));
}
}  // namespace kernel

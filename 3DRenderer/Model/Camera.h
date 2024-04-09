#pragma once

#include <Eigen/Dense>
#include "Vertex.h"
#include "Primitives.h"
#include <array>
namespace renderer {
class Camera {
    using Matrix4d = Eigen::Matrix4d;

public:
    Camera(int width, int height);
    Eigen::Matrix<double, 3, 4> ApplyPerspectiveTransformation(
        const Eigen::Matrix<double, 3, 4> &) const;
    const Eigen::Matrix<double, 5, 4> &GetClippingPlanes() const;

private:
    void InitPerspective();
    void InitPlanes();

    static constexpr double kDefaultNearPlaneDistance = 2;

    Eigen::Matrix<double, 5, 4> planes_;
    double r_, l_, t_, b_;  // not sure what are these
    Matrix4d perspective_matrix_ = Matrix4d::Zero();
    double near_plane_distance_ = kDefaultNearPlaneDistance;
    double fov_ = -1;

    // double far_plane_distance_ = 100; idk if needed
};
}  // namespace renderer

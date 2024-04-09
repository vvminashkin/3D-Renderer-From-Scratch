//
// Created by supernyan on 07.01.24.
//
#include "Camera.h"
#include <cassert>
namespace renderer {
Camera::Camera(int width, int height) {
    double dwidth = static_cast<double>(width);
    double dheight = static_cast<double>(height);
    assert("correct width and height given to camera" && dwidth > 0 && dheight > 0);
    double aspect_ratio = dheight / dwidth;
    l_ = -1;
    r_ = 1;
    b_ = -aspect_ratio;
    t_ = -b_;

    // eq. 5.27 mathematics for 3d game...
    fov_ = 2.0 * std::atan(1 / near_plane_distance_);
    InitPerspective();
    InitPlanes();
}
Eigen::Matrix<double, 3, 4> Camera::ApplyPerspectiveTransformation(
    const Eigen::Matrix<double, 3, 4>& vertices) const {
    Eigen::Matrix<double, 3, 4> ans = vertices;
    ans = (perspective_matrix_ * ans.transpose()).transpose();
    for (int i = 0; i < 3; ++i) {
        assert("in perspetive transformation w coordinate is not zero" && ans.row(i).w() != 0);
        ans.row(i).topLeftCorner<1, 3>() /= ans.row(i).w();
    }
    return ans;
}
const Eigen::Matrix<double, 5, 4>& Camera::GetClippingPlanes() const {
    return planes_;
}
void renderer::Camera::InitPerspective() {
    // page 116 mathematics for 3d game...
    perspective_matrix_(0, 0) = (2.0 * near_plane_distance_ / (r_ - l_));
    perspective_matrix_(0, 2) = ((r_ + l_) / (r_ - l_));

    perspective_matrix_(1, 1) = (2 * near_plane_distance_ / (t_ - b_));
    perspective_matrix_(1, 2) = ((t_ + b_) / (t_ - b_));
    perspective_matrix_(2, 2) = -1;
    perspective_matrix_(2, 3) = -2.0 * near_plane_distance_;
    perspective_matrix_(3, 2) = -1;
}
void Camera::InitPlanes() {
    // Table 5.1 mathematics for 3d game...
    double focal_length = near_plane_distance_;  // for now
    double x_norm_coef = std::sqrt(focal_length * focal_length + 1);
    double y_norm_coef = std::sqrt(focal_length * focal_length + t_ * t_);
    planes_ << 0, 0, -1.0, near_plane_distance_, focal_length / x_norm_coef, 0, -1.0 / x_norm_coef,
        0, -focal_length / x_norm_coef, 0, -1.0 / x_norm_coef, 0, 0, focal_length / y_norm_coef,
        -t_ / y_norm_coef, 0, 0, -focal_length / y_norm_coef, -t_ / y_norm_coef, 0;
}
}  // namespace renderer

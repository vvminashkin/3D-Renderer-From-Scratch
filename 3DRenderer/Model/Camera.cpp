//
// Created by supernyan on 07.01.24.
//

#include "Camera.h"

namespace renderer {
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
void Camera::ApplyPerspectiveTransformation(std::array<Vertex, 3>* vertices) const {
    for (auto& i : *vertices) {
        i.coordinates = perspective_matrix_ * i.coordinates.GetHomogeneousCoordinates();
        i.normal = perspective_matrix_ * i.normal.GetHomogeneousCoordinates();
    }
}
}  // namespace renderer

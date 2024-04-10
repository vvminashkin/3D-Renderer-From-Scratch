#include "Object.h"
void renderer::BasicObject::AddTriangle(const BasicObject::Matrix3d& coordinates) {
    mesh_.AddTriangle(coordinates);
}
renderer::BasicObject::BasicObject(
    std::function<RGB(const Triangle&, const Eigen::Vector3d&)> function)
    : mesh_(function) {
}

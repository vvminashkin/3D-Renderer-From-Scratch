#include "Primitives.h"
#include <algorithm>
namespace renderer {
Triangle::Vector3d Triangle::CalculateCoordinatesFromBarycentric(
    const Vector3d &coordinates) const {
    Vector3d ans = Vector3d::Zero();
    ans = verticies_.x().coordinates.GetCoordinates() * coordinates.x() +
          verticies_.y().coordinates.GetCoordinates() * coordinates.y() +
          verticies_.z().coordinates.GetCoordinates() * coordinates.z();
    return ans;
}
BarycentricCoordinateSystem::BarycentricCoordinateSystem(const Triangle &original,
                                                         const Triangle &transformed)
    : original_(original), transformed_(transformed) {
}
Eigen::Vector3<BarycentricCoordinateSystem::BCoordinates>
BarycentricCoordinateSystem::GetTriangle() {
    return {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
}
BarycentricCoordinateSystem::Vector3d BarycentricCoordinateSystem::GetOriginalCoordinates(
    const BCoordinates &coordinates) const {
    return original_.CalculateCoordinatesFromBarycentric(coordinates);
}
BarycentricCoordinateSystem::Vector3d BarycentricCoordinateSystem::GetTransformedCoordinates(
    const BCoordinates &coordinates) const {
    return transformed_.CalculateCoordinatesFromBarycentric(coordinates);
}
Eigen::Vector3<Vertex> &Triangle::GetVerticies() {
    return verticies_;
}
const Eigen::Vector3<Vertex> &Triangle::GetVerticies() const {
    return verticies_;
}
}  // namespace renderer

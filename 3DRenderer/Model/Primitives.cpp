#include "Primitives.h"
#include <algorithm>
#include <iostream>
namespace renderer {
Triangle::Triangle(Matrix3d coordinates,
                   const std::function<RGB(const Triangle &, Vector3d)> *color_function_p)
    : color_function_p_(color_function_p) {
    assert("color function is nullptr" && color_function_p);
    verticies_.x().coordinates = coordinates.row(0);
    verticies_.y().coordinates = coordinates.row(1);
    verticies_.z().coordinates = coordinates.row(2);
}
Triangle::Vector3d Triangle::CalculateCoordinatesFromBarycentric(
    const Vector3d &coordinates) const {
    Vector3d ans = Vector3d::Zero();
    ans = verticies_.x().coordinates.GetCoordinates() * coordinates.x() +
          verticies_.y().coordinates.GetCoordinates() * coordinates.y() +
          verticies_.z().coordinates.GetCoordinates() * coordinates.z();
    return ans;
}
RGB Triangle::GetColor(Vector3d b_coordinate) const {
    return (*color_function_p_)(*this, b_coordinate);
}
void Triangle::SetColorFunction(
    const std::function<RGB(const Triangle &, Vector3d)> *color_function_p) {
    color_function_p_ = color_function_p;
}
BarycentricCoordinateSystem::BarycentricCoordinateSystem(const Triangle &original,
                                                         const Matrix34d &transformed)
    : original_(original), transformed_coordinates_matrix_(transformed) {
    for (int i = 0; i < 3; ++i) {
        original_coordinates_matrix_.row(i) =
            original_.GetVerticies()[i].coordinates.GetHomogeneousCoordinates();
    }

    barycentric_transformation_matrix_ =
        transformed_coordinates_matrix_.topLeftCorner<2, 2>().transpose();
    barycentric_transformation_matrix_.col(0) -=
        transformed_coordinates_matrix_.row(2).topLeftCorner<1, 2>();
    barycentric_transformation_matrix_.col(1) -=
        transformed_coordinates_matrix_.row(2).topLeftCorner<1, 2>();
}
BarycentricCoordinateSystem::Vector3d BarycentricCoordinateSystem::GetOriginalCoordinates(
    const BCoordinates &coordinates) const {
    return original_coordinates_matrix_.topLeftCorner<3, 3>() * coordinates;
}
BarycentricCoordinateSystem::Vector4d BarycentricCoordinateSystem::GetTransformedCoordinates(
    const BCoordinates &coordinates) const {
    // std::cout<<transformed_coordinates_matrix_<<std::endl;
    // std::cout<<coordinates<<std::endl;
    return static_cast<Vector4d>(coordinates.transpose() * transformed_coordinates_matrix_);
}
Eigen::Vector3<Vertex> &Triangle::GetVerticies() {
    return verticies_;
}
const Eigen::Vector3<Vertex> &Triangle::GetVerticies() const {
    return verticies_;
}
Triangle::Matrix3d Triangle::GetVerticiesCoordinates() const {
    Matrix3d ans;
    ans.row(0) = verticies_.x().coordinates.GetCoordinates();
    ans.row(1) = verticies_.y().coordinates.GetCoordinates();
    ans.row(2) = verticies_.z().coordinates.GetCoordinates();
    return ans;
}
BarycentricCoordinateSystem::Matrix3d BarycentricCoordinateSystem::GetTriangleCoordinates(
    const Eigen::Vector3<BarycentricCoordinateSystem::BCoordinates> &triangle) const {
    Matrix3d ans;
    ans.row(0) = GetOriginalCoordinates(triangle.x());
    ans.row(1) = GetOriginalCoordinates(triangle.y());
    ans.row(2) = GetOriginalCoordinates(triangle.z());
    return ans;
}
double BarycentricCoordinateSystem::InterpolateZCoordinate(
    const BarycentricCoordinateSystem::BCoordinates &coordinates) {
    return transformed_coordinates_matrix_.col(3).dot(coordinates);
}
RGB BarycentricCoordinateSystem::GetColor(Vector3d b_coordinate) const {
    return original_.GetColor(b_coordinate);
}
BarycentricCoordinateSystem::Vector3d BarycentricCoordinateSystem::ConvertToBarycentricCoordinates(
    Eigen::Vector2d vec) const {
    vec -= transformed_coordinates_matrix_.row(2).topLeftCorner<1, 2>();
    Vector3d ans;
    ans.topLeftCorner<2, 1>() = barycentric_transformation_matrix_ * vec;
    ans.z() = 1 - ans.x() - ans.y();
    return ans;
}
Triangle::Matrix34d Triangle::GetVerticesHomogeniousCoordinates() const {
    Matrix34d ans;
    for (int i = 0; i < 3; ++i) {
        ans.row(i) = verticies_[i].coordinates.GetHomogeneousCoordinates();
    }
    return ans;
}
}  // namespace renderer

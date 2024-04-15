#include "Primitives.h"
#include <algorithm>
#include <iostream>
#include <cassert>

namespace renderer {
Triangle::Triangle(Matrix3d coordinates) {
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
RGB Triangle::GetAmbientColor(const Vector3d &b_coordinate) const {
    return (*ambient_color_function_p_)(*this, b_coordinate);
}
RGB Triangle::GetDiffuseColor(const Vector3d &b_coordinate) const {
    if (!diffuse_color_function_p_) {
        return GetAmbientColor(b_coordinate);
    }
    return (*diffuse_color_function_p_)(*this, b_coordinate);
}
RGB Triangle::GetSpecularColor(const Vector3d &b_coordinate) const {
    if (!specular_color_function_p_) {
        return {1, 1, 1};
    }
    return (*specular_color_function_p_)(*this, b_coordinate);
}
void Triangle::SetColorFunction(const ColorFunction *ambient, const ColorFunction *diffuse,
                                const ColorFunction *specular) {
    ambient_color_function_p_ = ambient;
    diffuse_color_function_p_ = diffuse;
    specular_color_function_p_ = specular;
}
void Triangle::CalculateNorm() {
    normal_ =
        (verticies_[0].coordinates.GetCoordinates() - verticies_[1].coordinates.GetCoordinates())
            .cross(verticies_[0].coordinates.GetCoordinates() -
                   verticies_[2].coordinates.GetCoordinates())
            .normalized();
}
Triangle::Vector3d Triangle::GetNormal(const Vector3d &b_coordinate) const {
    if (!normal_function_p_) {
        return normal_;
    }
    return (*normal_function_p_)(*this, b_coordinate);
}
BarycentricCoordinateSystem::BarycentricCoordinateSystem(const Triangle &original,
                                                         const Matrix34d &transformed)
    : original_(original), transformed_coordinates_matrix_(transformed) {
    for (int i = 0; i < 3; ++i) {
        original_coordinates_matrix_.row(i) =
            original_.GetVerticies()[i].coordinates.GetHomogeneousCoordinates();
    }

    barycentric_transformation_matrix_ =
        MakeBarycentricTransformationMatrix(transformed_coordinates_matrix_.topLeftCorner<3, 3>());
}
BarycentricCoordinateSystem::Vector3d BarycentricCoordinateSystem::GetOriginalCoordinates(
    const BCoordinates &coordinates) const {
    return coordinates.transpose() * original_coordinates_matrix_.topLeftCorner<3, 3>();
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
BarycentricCoordinateSystem::Matrix2d
BarycentricCoordinateSystem::MakeBarycentricTransformationMatrix(const Matrix3d &coordinates) {
    Matrix2d inverse_matrix;
    inverse_matrix = coordinates.topLeftCorner<2, 2>().transpose();
    inverse_matrix.col(0) -= coordinates.row(2).topLeftCorner<1, 2>();
    inverse_matrix.col(1) -= coordinates.row(2).topLeftCorner<1, 2>();
    inverse_matrix = inverse_matrix.inverse().eval();
    return inverse_matrix;
}
BarycentricCoordinateSystem::Vector3d BarycentricCoordinateSystem::TransformToBarycentric(
    const Matrix2d &transform_matrix, const Matrix3d &coordinates, const Vector2d &point) {

    Vector3d ans;
    ans.topLeftCorner<2, 1>() = point;
    ans.topLeftCorner<2, 1>() -= coordinates.row(2).topLeftCorner<1, 2>();
    ans.topLeftCorner<2, 1>() = transform_matrix * ans.topLeftCorner<2, 1>();
    ans.z() = 1 - ans.x() - ans.y();
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
RGB BarycentricCoordinateSystem::GetColor(const Vector3d &b_coordinate) const {
    return original_.GetAmbientColor(b_coordinate);
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
const BarycentricCoordinateSystem::Matrix34d &
BarycentricCoordinateSystem::GetOriginalCoordinatesMatrix() const {
    return original_coordinates_matrix_;
}
const Triangle &BarycentricCoordinateSystem::GetOriginalTriangle() const {
    return original_;
}
}  // namespace renderer

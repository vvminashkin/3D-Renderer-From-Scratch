#pragma once

#include "Utils.h"
#include <Eigen/Dense>
#include "Vertex.h"

namespace renderer {
class Triangle {
public:
    using Vector3d = Eigen::Vector3d;
    using Matrix34d = Eigen::Matrix<double, 3, 4>;
    using Matrix3d = Eigen::Matrix3d;
    Triangle() = default;
    Triangle(Eigen::Matrix3d, const std::function<RGB(const Triangle &, const Vector3d &)> *);
    Vector3d CalculateCoordinatesFromBarycentric(const Vector3d &) const;
    Eigen::Vector3<Vertex> &GetVerticies();
    const Eigen::Vector3<Vertex> &GetVerticies() const;
    Matrix3d GetVerticiesCoordinates() const;
    Matrix34d GetVerticesHomogeniousCoordinates() const;
    RGB GetColor(const Vector3d &b_coordinate) const;
    void SetColorFunction(const std::function<RGB(const Triangle &, const Vector3d &)> *);

private:
    Eigen::Vector3<Vertex> verticies_;
    const std::function<RGB(const Triangle &, const Vector3d &)> *color_function_p_ = nullptr;
};
class BarycentricCoordinateSystem {
public:
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;
    using Vector2d = Eigen::Vector2d;
    using BCoordinates = Vector3d;
    using Matrix34d = Eigen::Matrix<double, 3, 4>;
    using Matrix2d = Eigen::Matrix2d;
    using Matrix3d = Eigen::Matrix3d;

    static Matrix2d MakeBarycentricTransformationMatrix(const Matrix3d &coordinates);

    static Vector3d TransformToBarycentric(const Matrix2d &transform_matrix,
                                           const Matrix3d &coordinates, const Vector2d &point);

    BarycentricCoordinateSystem(const Triangle &original, const Matrix34d &transformed);
    Vector3d GetOriginalCoordinates(const BCoordinates &) const;
    Vector4d GetTransformedCoordinates(const BCoordinates &) const;
    Matrix3d GetTriangleCoordinates(const Eigen::Vector3<BCoordinates> &) const;
    double InterpolateZCoordinate(const BCoordinates &);
    RGB GetColor(const Vector3d &b_coordinate) const;
    Vector3d ConvertToBarycentricCoordinates(Eigen::Vector2d) const;
    const Matrix34d &GetOriginalCoordinatesMatrix() const;

private:
    Triangle original_;
    Matrix34d original_coordinates_matrix_;
    Matrix34d transformed_coordinates_matrix_;
    Matrix2d barycentric_transformation_matrix_;
};
}  // namespace renderer

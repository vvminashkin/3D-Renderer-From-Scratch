#pragma once

#include <Eigen/Dense>

#include "Utils.h"
#include "Vertex.h"
namespace renderer {
class Triangle {
public:
    using Vector3d = Eigen::Vector3d;
    Vector3d CalculateCoordinatesFromBarycentric(const Vector3d &) const;
    Eigen::Vector3<Vertex> &GetVerticies();
    const Eigen::Vector3<Vertex> &GetVerticies() const;
    Eigen::Vector3<Vector3d> GetVerticiesCoordinates() const;

private:
    Eigen::Vector3<Vertex> verticies_;
};
class BarycentricCoordinateSystem {
public:
    using Vector3d = Eigen::Vector3d;
    using BCoordinates = Vector3d;
    Eigen::Vector3<BCoordinates> GetTriangle();
    BarycentricCoordinateSystem(const Triangle &original, const Triangle &transformed);
    Vector3d GetOriginalCoordinates(const BCoordinates &) const;
    Vector3d GetTransformedCoordinates(const BCoordinates &) const;

private:
    Triangle original_;
    Triangle transformed_;
};
}  // namespace renderer

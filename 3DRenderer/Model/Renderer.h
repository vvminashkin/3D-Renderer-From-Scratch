#pragma once

#include <Eigen/Dense>
#include "Eigen/src/Core/Matrix.h"
#include "Eigen/src/Geometry/Quaternion.h"
#include "Primitives.h"
#include "World.h"
#include "Screen.h"

namespace model {
class Model;
}
namespace renderer {
class Renderer {

public:
    using Matrix4d = Eigen::Matrix4d;
    using Quaterniond = Eigen::Quaterniond;
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;
    using Matrix3d = Eigen::Matrix3d;
    using Matrix34d = Eigen::Matrix<double, 3, 4>;
    friend class model::Model;
    Renderer() = default;

    std::unique_ptr<Screen> Draw(const World &world, size_t width, size_t height);

    void ShiftTriangleCoordinates(const World::ObjectHolder &owner, Triangle *);
    void ShiftTriangleToAlignCamera(const World &, Triangle *);
    void DrawTriangle(const Mesh::ITriangle &current, const World::ObjectHolder &owner_object,
                      const World &world, Screen *screen);

    void RasterizeTriangle(const BarycentricCoordinateSystem &, const Eigen::Matrix3d &, Screen *);

    static bool DetermineSide(const Eigen::Vector4d &plane, const Eigen::Vector3d &point);
    static Eigen::Vector3d PlaneLineIntersection(const Eigen::Vector4d &plane,
                                                 const Eigen::Vector3d &point1,
                                                 const Eigen::Vector3d &point2);
    // true if clipped
    static bool ClipOneTriangle(const Eigen::Vector4d &plane, const Eigen::Matrix3d &triangle,
                                std::list<Eigen::Matrix3d> *triangles);
    static void ClipAllTriangles(const Eigen::Vector4d &plane,
                                 std::list<Eigen::Matrix3d> *triangles);

private:
    static Matrix34d MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                         const Vector3d &offset);
    static void ApplyMatrix(const Matrix34d &, Triangle *);
};
}  // namespace renderer

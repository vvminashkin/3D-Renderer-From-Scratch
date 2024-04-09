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
    using Matrix4d = Eigen::Matrix4d;
    using Quaterniond = Eigen::Quaterniond;
    using Vector3d = Eigen::Vector3d;

public:
    friend class model::Model;
    Renderer() = default;

    std::unique_ptr<Screen> Draw(const World &world, size_t width, size_t height);

    void ShiftTriangleCoordinates(const World::ObjectHolder &owner, Triangle *);
    void ShiftTriangleToAlignCamera(const World &, Triangle *);
    void DrawTriangle(const Mesh::ITriangle &current, const World::ObjectHolder &owner_object,
                      const World &world, Screen *screen);

    void RasterizeTriangle(const BarycentricCoordinateSystem &, const Eigen::Matrix3d &, Screen *);

private:
    static Matrix4d MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                        const Vector3d &offset);
    static void ApplyHomogeneousTransformationMatrix(const Eigen::Matrix4d &, Triangle *);
};
}  // namespace renderer

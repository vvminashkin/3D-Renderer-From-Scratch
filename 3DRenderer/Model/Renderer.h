#pragma once

#include <Eigen/Dense>
#include "Eigen/src/Core/Matrix.h"
#include "Eigen/src/Geometry/Quaternion.h"
#include "World.h"
#include "Screen.h"
namespace renderer {
class Renderer {
    using Matrix4d = Eigen::Matrix4d;
    using Quaterniond = Eigen::Quaterniond;
    using Vector3d = Eigen::Vector3d;

public:
    Renderer() = default;

    Screen Draw(const World &world, size_t width, size_t height);

private:
    void ShiftTriangleCoordinates(const World::ObjectHolder &owner, std::array<Vertex, 3> *);
    void ShiftTriangleToAlignCamera(const World &, std::array<Vertex, 3> *);
    void DrawTriangle(const Triangle &current, const World::ObjectHolder &owner_object,
                      const World &world, Screen &screen);
    static Matrix4d MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                        const Vector3d &offset);
    static void ApplyHomogeneousTransformationMatrix(const Eigen::Matrix4d &,
                                                     std::array<Vertex, 3> *);
};
}  // namespace renderer

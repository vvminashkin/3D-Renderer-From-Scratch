#pragma once

#include <Eigen/Dense>
#include "World.h"
#include "Screen.h"
namespace renderer {
class Renderer {
    using Matrix4d = Eigen::Matrix4d;

public:
    Renderer(const Screen &);

    void Draw();

private:
    void ShiftTriangleCoordinates(const World::ObjectHolder &owner, std::array<Vertex, 3> *);
    void ShiftTriangleToAlignCamera(std::array<Vertex, 3> *);
    void DrawTriangle(const Triangle &, const World::ObjectHolder &);
    static Matrix4d MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                        const Vector3d &offset);
    static void ApplyHomogeneousTransformationMatrix(const Eigen::Matrix4d &,
                                                     std::array<Vertex, 3> *);
    World world_;
    Screen screen_;
};
}  // namespace renderer

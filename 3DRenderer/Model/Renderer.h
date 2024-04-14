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
    // coordinates of light sources shifted to align camera
    Renderer() = default;

    struct LightSourcesDescription {
        std::vector<Vector3d> point_light_coordinates_;
    };
    std::unique_ptr<Screen> Draw(const World &world, size_t width, size_t height);
    void ShiftLightToAlignCamera(const World &, LightSourcesDescription *desc);
    void ShiftTriangleCoordinates(const World::ObjectHolder &owner, Triangle *);
    void ShiftTriangleToAlignCamera(const World &, Triangle *);
    void DrawTriangle(const Mesh::ITriangle &current, const World::ObjectHolder &owner_object,
                      const World &world, const LightSourcesDescription &, Screen *screen);

    void RasterizeTriangle(const BarycentricCoordinateSystem &, const Eigen::Matrix3d &,
                           const World &, const LightSourcesDescription &, Screen *);

    static bool DetermineSide(const Eigen::Vector4d &plane, const Eigen::Vector3d &point);
    static Eigen::Vector3d PlaneLineIntersection(const Eigen::Vector4d &plane,
                                                 const Eigen::Vector3d &point1,
                                                 const Eigen::Vector3d &point2);
    // true if clipped
    static bool ClipOneTriangle(const Eigen::Vector4d &plane, const Eigen::Matrix3d &triangle,
                                std::list<Eigen::Matrix3d> *triangles);
    static void ClipAllTriangles(const Eigen::Vector4d &plane,
                                 std::list<Eigen::Matrix3d> *triangles);

    RGB CalculateBlinnPhong(const RGB &initial_color, const LightSourcesDescription &desc,
                            const World &world, const Vector3d &b_coordinates,
                            const Triangle &triangle);
    RGB CalculateBlinnPhongDiffusion(const RGB &initial_color, const LightSourcesDescription &desc,
                                     const World &world, const Vector3d &coordinates,
                                     const Vector3d &normal);

private:
    static Matrix34d MakeHomogeneousTransformationMatrix(const Quaterniond &rotation,
                                                         const Vector3d &offset);
    static void ApplyMatrix(const Matrix34d &, Triangle *);
};
}  // namespace renderer

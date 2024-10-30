#pragma once
#include "Primitives.h"
#include "Vertex.h"
#include <Eigen/Dense>
#include <vector>
#include <functional>

namespace renderer {
class Mesh {
public:
    using Vector3i = Eigen::Vector3i;
    using Vector3d = Eigen::Vector3d;
    using ColorFuntion = std::function<RGB(const Triangle &, const Vector3d &)>;
    Mesh(ColorFuntion ambient, ColorFuntion diffuse, ColorFuntion specular);
    struct ITriangle {
        Vector3i points_;
    };
    using VerticesConstIterator = std::vector<Vertex>::const_iterator;
    using TrianglesConstIterator = std::vector<ITriangle>::const_iterator;
    void SetNormalFunction(
        std::function<Vector3d(const Triangle &, const Vector3d &)> normal_function);
    Iterable<VerticesConstIterator> GetVertices() const;

    Iterable<TrianglesConstIterator> GetTriangles() const;

    Triangle MakeTriangleVertices(const ITriangle &) const;
    void AddTriangle(const Eigen::Matrix3d &);
    void AddTriangle(const Eigen::Matrix3d &, const Eigen::Matrix3d &);

private:
    std::vector<Vertex> vertices_;
    std::vector<ITriangle> triangles_;
    std::function<RGB(const Triangle &, const Vector3d &)> ambient_color_function_;
    std::function<RGB(const Triangle &, const Vector3d &)> diffuse_color_function_;
    std::function<RGB(const Triangle &, const Vector3d &)> specular_color_function_;
    std::function<Vector3d(const Triangle &, const Vector3d &)> normal_function_;
};
}  // namespace renderer

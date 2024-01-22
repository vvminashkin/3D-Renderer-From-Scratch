#pragma once
#include "Primitives.h"
#include "Vertex.h"
#include <Eigen/Dense>
#include <vector>
namespace renderer {
class Mesh {
    using VerticesConstIterator = decltype(std::declval<const std::vector<Vertex>>().begin()); // TODO 
    using TrianglesConstIterator = decltype(std::declval<const std::vector<Triangle>>().begin());

public:
    Iterable<VerticesConstIterator> GetVerticesIterable() const;

    Iterable<TrianglesConstIterator> GetTrianglesIterable() const;

    std::array<Vertex, 3> GetTriangleVertices(const Triangle &) const;

private:
    std::vector<Vertex> vertices_;
    std::vector<Triangle> triangles_;
};
}  // namespace renderer

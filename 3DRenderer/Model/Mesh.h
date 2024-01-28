#pragma once
#include "Primitives.h"
#include "Vertex.h"
#include <Eigen/Dense>
#include <vector>
namespace renderer {
class Mesh {
    using VerticesConstIterator = std::vector<Vertex>::const_iterator;
    using TrianglesConstIterator = std::vector<Triangle>::const_iterator;

public:
    Iterable<VerticesConstIterator> GetVertices() const;

    Iterable<TrianglesConstIterator> GetTriangles() const;

    std::array<Vertex, 3> GetTriangleVertices(const Triangle &) const;

private:
    std::vector<Vertex> vertices_;
    std::vector<Triangle> triangles_;
};
}  // namespace renderer

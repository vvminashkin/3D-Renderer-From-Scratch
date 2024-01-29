#include "Mesh.h"

namespace renderer {
Iterable<Mesh::TrianglesConstIterator> Mesh::GetTriangles() const {
    return Iterable(triangles_.begin(), triangles_.end());
}

Iterable<Mesh::VerticesConstIterator> Mesh::GetVertices() const {
    return Iterable(vertices_.begin(), vertices_.end());
}
std::array<Vertex, 3> Mesh::GetTriangleVertices(const Triangle &triangle) const {

    std::array<Vertex, 3> ans;
    for (size_t i = 0; i < 3; ++i) {
        ans[i] = vertices_[triangle.GetPoints()(i)];  // TODO get triangle
    }

    return ans;
}

}  // namespace renderer

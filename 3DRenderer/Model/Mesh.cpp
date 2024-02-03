#include "Mesh.h"

namespace renderer {
Iterable<Mesh::TrianglesConstIterator> Mesh::GetTriangles() const {
    return Iterable(triangles_.begin(), triangles_.end());
}

Iterable<Mesh::VerticesConstIterator> Mesh::GetVertices() const {
    return Iterable(vertices_.begin(), vertices_.end());
}
Triangle Mesh::GetTriangleVertices(const ITriangle &triangle) const {

    Triangle ans;
    for (size_t i = 0; i < 3; ++i) {
        ans.verticies_[i] = vertices_[triangle.points_(i)];  // TODO get triangle
    }

    return ans;
}

}  // namespace renderer

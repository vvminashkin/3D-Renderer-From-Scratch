#include "Mesh.h"

namespace renderer {
Iterable<Mesh::TrianglesConstIterator> Mesh::GetTrianglesIterable() const {
    return Iterable(triangles_.begin(), triangles_.end());
}

Iterable<Mesh::VerticesConstIterator> Mesh::GetVerticesIterable() const {
    return Iterable(vertices_.begin(), vertices_.end());
}
}  // namespace renderer

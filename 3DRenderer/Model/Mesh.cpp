#include "Mesh.h"
namespace renderer{
const std::vector<Vertex> &Mesh::GetVertices() const {
    return vertices_;
}

    const std::vector<Triangle> &Mesh::GetTriangles() const {
        return triangles_;
    }
}

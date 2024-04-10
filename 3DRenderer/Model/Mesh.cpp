#include "Mesh.h"

namespace renderer {
Mesh::Mesh(std::function<RGB(const Triangle&, const Vector3d&)> color_function)
    : color_function_(color_function) {
}
Iterable<Mesh::TrianglesConstIterator> Mesh::GetTriangles() const {
    return Iterable(triangles_.begin(), triangles_.end());
}

Iterable<Mesh::VerticesConstIterator> Mesh::GetVertices() const {
    return Iterable(vertices_.begin(), vertices_.end());
}
Triangle Mesh::MakeTriangleVertices(const ITriangle& triangle) const {

    Triangle ans;
    for (size_t i = 0; i < 3; ++i) {
        ans.GetVerticies()[i] = vertices_[triangle.points_(i)];  // TODO get triangle
    }
    ans.SetColorFunction(&color_function_);

    return ans;
}

void Mesh::AddTriangle(const Eigen::Matrix3d& coordinates) {
    Eigen::Vector3d v1 = coordinates.row(1) - coordinates.row(0);
    Eigen::Vector3d v2 = coordinates.row(2) - coordinates.row(0);
    Eigen::Vector3d normal = v1.cross(v2);
    normal.normalize();
    int current_ind = vertices_.size();
    for (int i = 0; i < 3; ++i) {
        vertices_.emplace_back(
            Vertex{.coordinates = static_cast<Eigen::Vector3d>(coordinates.row(i).eval()),
                   .normal = normal});
    }

    triangles_.push_back(ITriangle{Vector3i{current_ind, current_ind + 1, current_ind + 2}});
}
}  // namespace renderer

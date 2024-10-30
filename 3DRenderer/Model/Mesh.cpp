#include "Mesh.h"

namespace renderer {
Mesh::Mesh(ColorFuntion ambient, ColorFuntion diffuse, ColorFuntion specular)
    : ambient_color_function_(ambient),
      diffuse_color_function_(diffuse),
      specular_color_function_(specular) {
}
Iterable<Mesh::TrianglesConstIterator> Mesh::GetTriangles() const {
    return Iterable(triangles_.begin(), triangles_.end());
}

Iterable<Mesh::VerticesConstIterator> Mesh::GetVertices() const {
    return Iterable(vertices_.begin(), vertices_.end());
}
Triangle Mesh::MakeTriangleVertices(const ITriangle &triangle) const {

    Triangle ans;
    for (size_t i = 0; i < 3; ++i) {
        ans.GetVerticies()[i] = vertices_[triangle.points_(i)];  // TODO get triangle
    }
    ans.SetColorFunction(&ambient_color_function_, &diffuse_color_function_,
                         &specular_color_function_);
    if (normal_function_) {
        ans.SetNormalFunction(&normal_function_);
    }

    return ans;
}

void Mesh::AddTriangle(const Eigen::Matrix3d &coordinates) {
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
void Mesh::AddTriangle(const Eigen::Matrix3d &coordinates, const Eigen::Matrix3d &normals) {
    int current_ind = vertices_.size();
    for (int i = 0; i < 3; ++i) {
        vertices_.emplace_back(
            Vertex{.coordinates = static_cast<Vector3d>(coordinates.row(i).eval()),
                   .normal = static_cast<Vector3d>(normals.row(i).normalized().eval())});
    }

    triangles_.push_back(ITriangle{Vector3i{current_ind, current_ind + 1, current_ind + 2}});
}
void Mesh::SetNormalFunction(
    std::function<Vector3d(const Triangle &, const Vector3d &)> normal_function) {
    normal_function_ = normal_function;
}
}  // namespace renderer

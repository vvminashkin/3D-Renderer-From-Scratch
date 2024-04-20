#pragma once

#include "Mesh.h"
#include <iostream>

namespace renderer {
class BasicObject {
public:
    using ColorFunction = std::function<RGB(const Triangle&, const Eigen::Vector3d&)>;
    using MeshConstIterator = std::vector<Mesh>::const_iterator;
    using Vector3d = Eigen::Vector3d;
    BasicObject();
    using Matrix3d = Eigen::Matrix3d;
    void AddTriangle(const Matrix3d&, size_t index = 0);
    void AddTriangle(const Matrix3d&, const Matrix3d&, size_t index = 0);
    Iterable<MeshConstIterator> GetMesh() const {
        return Iterable<MeshConstIterator>(meshes_.begin(), meshes_.end());
    }
    void AddMesh(RGB ambient, RGB diffuse, RGB specular) {
        meshes_.emplace_back(
            [ambient](const Triangle&, const Vector3d& b_coordinates) { return ambient; },

            [diffuse](const Triangle&, const Vector3d& b_coordinates) { return diffuse; },

            [specular](const Triangle&, const Vector3d& b_coordinates) { return specular; });
    }
    Mesh& GetMesh(size_t index = 0);

private:
    std::vector<Mesh> meshes_;
};
class Sphere {

    using ColorFunction = std::function<RGB(const Triangle&, const Eigen::Vector3d&)>;
    using MeshConstIterator = std::vector<Mesh>::const_iterator;
    using Vector3d = Eigen::Vector3d;
    Sphere();
    using Matrix3d = Eigen::Matrix3d;
    void AddTriangle(const Matrix3d&, size_t index = 0);
    void AddTriangle(const Matrix3d&, const Matrix3d&, size_t index = 0);
    Iterable<MeshConstIterator> GetMesh() const {
        return Iterable<MeshConstIterator>(meshes_.begin(), meshes_.end());
    }
    void AddMesh(RGB ambient, RGB diffuse, RGB specular) {
        meshes_.emplace_back(
            [ambient](const Triangle&, const Vector3d& b_coordinates) { return ambient; },

            [diffuse](const Triangle&, const Vector3d& b_coordinates) { return diffuse; },

            [specular](const Triangle&, const Vector3d& b_coordinates) { return specular; });
    }
    Mesh& GetMesh(size_t index = 0);

private:
    std::vector<Mesh> meshes_;
};
}  // namespace renderer

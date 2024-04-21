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
    Iterable<MeshConstIterator> GetMeshes() const;
    void AddMesh(RGB ambient, RGB diffuse, RGB specular);
    Mesh& GetMesh(size_t index = 0);

private:
    std::vector<Mesh> meshes_;
};
class Sphere {
public:
    using ColorFunction = std::function<RGB(const Triangle&, const Eigen::Vector3d&)>;
    using MeshConstIterator = std::vector<Mesh>::const_iterator;
    using Vector3d = Eigen::Vector3d;
    Sphere(RGB ambient, RGB diffuse, RGB specular, double radius = 1.0,
           size_t max_triangle_count = 320);
    using Matrix3d = Eigen::Matrix3d;
    Iterable<MeshConstIterator> GetMeshes() const;
    Mesh& GetMesh(size_t index = 0);

private:
    static std::list<Matrix3d> MakeIcosahedron();
    static void SubDivideAndExtrude(Matrix3d, std::list<Matrix3d>*);
    static Matrix3d MakeMatrix(Vector3d, Vector3d, Vector3d);
    void AddMesh(RGB ambient, RGB diffuse, RGB specular);
    std::vector<Mesh> meshes_;
};
}  // namespace renderer

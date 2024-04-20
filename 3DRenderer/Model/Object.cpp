#include "Object.h"
void renderer::BasicObject::AddTriangle(const BasicObject::Matrix3d& coordinates, size_t index) {
    meshes_[index].AddTriangle(coordinates);
}
void renderer::BasicObject::AddTriangle(const BasicObject::Matrix3d& coordinates,
                                        const Matrix3d& normals, size_t index) {
    meshes_[index].AddTriangle(coordinates, normals);
}
renderer::BasicObject::BasicObject() {
}
renderer::Mesh& renderer::BasicObject::GetMesh(size_t index) {
    return meshes_[index];
}

#include "Object.h"
void renderer::BasicObject::AddTriangle(const BasicObject::Matrix3d& coordinates, size_t index) {
    meshes_[index].AddTriangle(coordinates);
}
renderer::BasicObject::BasicObject() {
}

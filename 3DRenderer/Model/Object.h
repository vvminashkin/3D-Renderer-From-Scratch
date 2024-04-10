#pragma once

#include "Mesh.h"
#include <iostream>

namespace renderer {
class BasicObject {
public:
    BasicObject(std::function<RGB(const Triangle &, const Eigen::Vector3d &)>);
    using Matrix3d = Eigen::Matrix3d;
    void AddTriangle(const Matrix3d &);
    const Mesh &GetMesh() const {
        return mesh_;
    }

private:
    Mesh mesh_;
};
}  // namespace renderer

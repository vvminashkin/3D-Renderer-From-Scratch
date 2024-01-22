#pragma once

#include "Mesh.h"
namespace renderer {
class BasicObject {
public:
    const Mesh &GetMesh() const {
        return mesh_;
    }

private:
    Mesh mesh_;
};
}  // namespace renderer

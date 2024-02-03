#pragma once

#include <Eigen/Dense>

#include "Utils.h"
#include "Vertex.h"
namespace renderer {
struct Triangle {
    Eigen::Vector3<Vertex> verticies_;
};
}  // namespace renderer

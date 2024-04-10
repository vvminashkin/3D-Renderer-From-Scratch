#pragma once

#include <Eigen/Dense>
#include "Utils.h"

namespace renderer {
struct Vertex {
    Point coordinates;
    Direction normal;
};

}  // namespace renderer

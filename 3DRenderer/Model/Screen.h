#pragma once
#include "Eigen/Dense"
#include "World.h"
namespace renderer {
class Screen {
public:

private:
    Eigen::Matrix<RGB, Eigen::Dynamic, Eigen::Dynamic> screen_matrix_;
};
}  // namespace renderer

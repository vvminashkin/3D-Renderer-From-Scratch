#pragma once
#include "Eigen/Dense"
#include "World.h"
namespace renderer {
class Screen {
public:
    Screen(size_t width, size_t height);
private:
    Eigen::Matrix<RGB, Eigen::Dynamic, Eigen::Dynamic> screen_matrix_;
};
}  // namespace renderer

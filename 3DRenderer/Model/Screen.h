#pragma once
#include "Eigen/Dense"
#include "World.h"

namespace renderer {
class Screen {
public:
    Screen(size_t width, size_t height);
    Screen(Screen &&) noexcept = default;
    Screen(const Screen &) = default;
    Screen &operator=(const Screen &) = default;
    Screen &operator=(Screen &&) noexcept = default;
    void SetPixel(size_t row, size_t column, const RGB &color);
    void SetZ(size_t row, size_t column, double z);
    double GetZ(size_t row, size_t column) const;
    const RGB &GetPixel(size_t row, size_t column) const;
    int GetWidth() const;
    int GetHeight() const;
    const Eigen::Matrix<RGB, Eigen::Dynamic, Eigen::Dynamic> &GetScreenMatrix() const;

private:
    Eigen::Matrix<RGB, Eigen::Dynamic, Eigen::Dynamic> screen_matrix_;
    Eigen::MatrixXd z_buffer_;
};
}  // namespace renderer

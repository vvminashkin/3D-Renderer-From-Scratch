#include "Screen.h"
#include <iostream>

namespace renderer {
Screen::Screen(size_t width, size_t height)
    : screen_matrix_(height, width), z_buffer_(height, width) {
    z_buffer_.setZero();
}
void Screen::SetPixel(size_t row, size_t column, const RGB &color) {
    screen_matrix_(row, column) = color;
}
void Screen::SetZ(size_t row, size_t column, double z) {
    z_buffer_(row, column) = z;
}
double Screen::GetZ(size_t row, size_t column) const {
    return z_buffer_(row, column);
}
const RGB &renderer::Screen::GetPixel(size_t row, size_t column) const {
    return screen_matrix_(row, column);
}
int Screen::GetWidth() const {
    return screen_matrix_.cols();
}

int Screen::GetHeight() const {
    return screen_matrix_.rows();
}
const Eigen::Matrix<renderer::RGB, Eigen::Dynamic, Eigen::Dynamic> &Screen::GetScreenMatrix()
    const {
    return screen_matrix_;
}
}  // namespace renderer

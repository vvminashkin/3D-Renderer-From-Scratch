#include "Utils.h"
namespace renderer {
RGB::RGB(const std::initializer_list<double>& init) {
    size_t i = 0;
    for (double num : init) {
        assert(i != 3);
        val_[i] = num;
        ++i;
    }
}
double renderer::RGB::GetR() const {
    return val_(0);
}
void renderer::RGB::SetR(double val) {
    val_(0) = val;
}
double renderer::RGB::GetG() const {
    return val_(1);
}
void renderer::RGB::SetG(double val) {
    val_(1) = val;
}
double renderer::RGB::GetB() const {
    return val_(2);
}
void renderer::RGB::SetB(double val) {
    val_(2) = val;
}
RGB::RGB() : val_(Vector3d::Zero()) {
}
Point::Vector3d renderer::Point::GetCoordinates() const {
    return data_;
}
Point& Point::operator=(const Vector3d& coordinates) {
    data_ = coordinates;
    return *this;
}
Point::Vector4d Point::GetHomogeneousCoordinates() const {
    Vector4d ans;
    ans.topLeftCorner<3, 1>() = data_;
    ans(3) = 1;
    return ans;
}
Point& Point::operator=(const Vector4d& h_coordinates) {
    data_ = h_coordinates.topLeftCorner<3, 1>();
    return *this;
}
Direction::Vector3d Direction::GetCoordinates() const {
    return data_;
}
Direction::Vector4d Direction::GetHomogeneousCoordinates() const {
    Vector4d ans;
    ans.topLeftCorner<3, 1>() = data_;
    ans(3) = 0;
    return ans;
}
Direction& Direction::operator=(const Vector3d& coordinates) {
    data_ = coordinates;
    return *this;
}
Direction& Direction::operator=(const Vector4d& h_coordinates) {
    data_ = h_coordinates.topLeftCorner<3, 1>();
    return *this;
}
Direction::Direction(const Vector3d& vec) : data_(vec) {
}
Direction::Direction(const Vector4d& vec) : data_(vec.topLeftCorner<3, 1>()) {
}
Point::Point(const Vector4d& vec) : data_(vec.topLeftCorner<3, 1>()) {
}
Point::Point(const Vector3d& vec) : data_(vec) {
}
}  // namespace renderer

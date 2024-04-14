#include "Utils.h"
#include <cassert>

namespace renderer {
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
uint8_t RGB::DoubleToInt(double val) {
    return std::min(std::max(static_cast<uint32_t>(val * 255.0), uint32_t{0}), uint32_t{255});
}
uint8_t RGB::GetRi() const {
    return DoubleToInt(GetR());
}
uint8_t RGB::GetGi() const {

    return DoubleToInt(GetG());
}
uint8_t RGB::GetBi() const {

    return DoubleToInt(GetB());
}
RGB RGB::operator+(const RGB& other) const {
    RGB ans = *this;
    ans.val_ += other.val_;
    return ans;
}
RGB RGB::operator*(const RGB& other) const {
    RGB ans = *this;
    ans.val_.cwiseProduct(other.val_);
    return ans;
}
RGB RGB::operator*(double a) const {
    RGB ans = *this;
    ans.val_ *= a;
    return ans;
}
RGB& RGB::operator+=(const RGB& other) {
    this->val_ += other.val_;
    return *this;
}
RGB& RGB::operator*=(const RGB& other) {
    val_ = this->val_.cwiseProduct(other.val_);
    return *this;
}
RGB& RGB::operator*=(double a) {
    val_ *= a;
    return *this;
}
Direction::Direction(const Vector3d& vec) : data_(vec) {
}
Direction::Direction(const Vector4d& vec) : data_(vec.topLeftCorner<3, 1>()) {
}
Direction& Direction::operator=(const Vector3d& coordinates) {
    data_ = coordinates;
    return *this;
}
Direction& Direction::operator=(const Vector4d& h_coordinates) {
    data_ = h_coordinates.topLeftCorner<3, 1>();
    return *this;
}
Direction::Vector3d Direction::GetCoordinates() const {
    return data_;
}
Direction::Vector4d Direction::GetHomogeneousCoordinates() const {
    Vector4d ans;
    ans.topLeftCorner<3, 1>() = data_;
    ans.w() = 0;
    return ans;
}
Point::Point(const Vector4d& vec) : data_(vec) {
}
Point::Point(const Vector3d& vec) {
    data_.topLeftCorner<3, 1>() = vec;
}

Point& Point::operator=(const Vector4d& h_coordinates) {
    data_ = h_coordinates;
    return *this;
}
Point& Point::operator=(const Vector3d& coordinates) {
    data_.topLeftCorner<3, 1>() = coordinates;
    data_.w() = 1;
    return *this;
}
Point::Vector3d renderer::Point::GetCoordinates() const {
    return data_.topLeftCorner<3, 1>();
}
const Point::Vector4d& Point::GetHomogeneousCoordinates() const {
    return data_;
}
double Point::w() {
    return data_.w();
}
}  // namespace renderer

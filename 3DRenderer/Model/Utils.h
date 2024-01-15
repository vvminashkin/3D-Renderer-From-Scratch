#pragma once
#include "Eigen/Dense"
namespace renderer {
template <typename Iterator>
class Iterable {
public:
    Iterable(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }
    Iterator begin() {  // NOLINT
        return begin_;
    }
    Iterator end() {  // NOLINT
        return end_;
    }

private:
    Iterator begin_;
    Iterator end_;
};

using Vector3d = Eigen::Vector3d;
using Vector4d = Eigen::Vector4d;
using Quaterniond = Eigen::Quaterniond;
class Direction {
public:
    Vector3d GetCoordinates() const;
    Vector4d GetHomogeneousCoordinates() const;
    template <typename T, typename F>
    Direction& operator=(const Eigen::Product<T, F>& evaluable) {
        return *this = evaluable.eval();
    }
    Direction& operator=(const Vector3d&);
    Direction& operator=(const Vector4d&);

private:
    Vector3d data_;
};

class Point {
public:
    Vector3d GetCoordinates() const;
    Vector4d GetHomogeneousCoordinates() const;
    template <typename T, typename F>
    Point& operator=(const Eigen::Product<T, F>& evaluable) {
        return *this = evaluable.eval();
    }
    Point& operator=(const Vector3d&);
    Point& operator=(const Vector4d&);

private:
    Vector3d data_;
};
}  // namespace renderer

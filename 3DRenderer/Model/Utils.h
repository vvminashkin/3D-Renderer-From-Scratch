#pragma once
#include "Eigen/Dense"
#include "Eigen/src/Core/Matrix.h"
#include <initializer_list>
#include <list>
#include <type_traits>

namespace renderer {
class RGB {
public:
    using Vector3d = Eigen::Vector3d;

    RGB(std::initializer_list<double> init) : val_{init} {
    }
    RGB() : val_{{0.0, 0.0, 0.0}} {
    }
    double GetR() const;
    void SetR(double);
    double GetG() const;
    void SetG(double);
    double GetB() const;
    void SetB(double);

    RGB operator+(const RGB &other) const;
    RGB operator*(const RGB &other) const;
    RGB operator*(double a) const;

    RGB &operator+=(const RGB &other);
    RGB &operator*=(const RGB &other);
    RGB &operator*=(double a);

    uint8_t GetRi() const;
    uint8_t GetGi() const;
    uint8_t GetBi() const;

private:
    // r,g,b
    static uint8_t DoubleToInt(double val);
    Vector3d val_;
};
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

class Direction {
public:
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;

    Direction() = default;
    Direction(const Vector3d &);
    Direction(const Vector4d &);
    template <typename T>
    Direction(const T &evaluable) {
        *this = evaluable.eval();
    }
    Vector3d GetCoordinates() const;
    Vector4d GetHomogeneousCoordinates() const;
    template <typename T>
    Direction &operator=(const T &evaluable) {
        return *this = evaluable.eval();
    }
    Direction &operator=(const Vector3d &);
    Direction &operator=(const Vector4d &);

private:
    Vector3d data_ = Vector3d::Zero();
};

class Point {
public:
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;
    Point() = default;
    Point(const Vector3d &);
    Point(const Vector4d &);

    Vector3d GetCoordinates() const;
    const Vector4d &GetHomogeneousCoordinates() const;
    template <typename T, class = typename std::enable_if<!std::is_same_v<Vector3d, T> &&
                                                          std::is_convertible_v<T, Vector3d>>>
    Point &operator=(const T &val) {
        return *this = static_cast<Vector3d>(val.eval());
    }

    template <typename T, typename F,
              class = typename std::enable_if<!std::is_same_v<Vector4d, T> &&
                                              !std::is_convertible_v<T, Vector3d> &&
                                              std::is_convertible_v<T, Vector4d>>>
    Point &operator=(const T &val) {
        return *this = static_cast<Vector4d>(val.eval());
    }
    double w();  // NOLINT
    Point &operator=(const Vector3d &);
    Point &operator=(const Vector4d &);

private:
    Vector4d data_ = Vector4d::Ones();
};

}  // namespace renderer

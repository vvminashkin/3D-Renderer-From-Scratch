#pragma once

#include <Eigen/Dense>
#include <memory>
#include "Mesh.h"
#include <QtCore>
namespace renderer {
class RGB {
public:
    double GetR() const;
    void SetR(double);
    double GetG() const;
    void SetG(double);
    double GetB() const;
    void SetB(double);

protected:
    // r,g,b
    Vector3d val_;
};

class BasicObject {
public:
    const Mesh &GetMesh() const {
        return mesh_;
    }

private:
    Mesh mesh_;
};

// Object for potential polymorphic use cases
class AnyObject {
public:
    class InnerBase;
    AnyObject() = default;

    AnyObject(const AnyObject &other) : inner_((other.inner_ ? other.inner_->Clone() : nullptr)) {
    }

    template <typename T>
    AnyObject(T &&object) : inner_(std::make_unique<Inner<T>>(std::forward<T>(object))) {
    }
    AnyObject(AnyObject &&) noexcept = default;
    AnyObject &operator=(AnyObject &&) noexcept = default;

    AnyObject &operator=(const AnyObject &val) {
        return *this = AnyObject(val);
    }

    const InnerBase *operator->() const {
        return inner_.get();
    }
    InnerBase *operator->() {
        return inner_.get();
    }

    class InnerBase {
    public:
        virtual ~InnerBase() = default;

        virtual std::unique_ptr<InnerBase> Clone() const = 0;

        virtual const Mesh &GetMesh() const = 0;
    };

private:
    template <typename T>
    class Inner : public InnerBase {
    public:
        Inner(const std::remove_reference_t<T> &value) : value_(value) {
        }
        Inner(T &&value) : value_(std::forward<T>(value)) {
        }

        std::unique_ptr<InnerBase> Clone() const override {
            return std::make_unique<Inner>(value_);
        }

        const Mesh &GetMesh() const override {
            return value_.GetMesh();
        }

    private:
        T value_;
    };

    std::unique_ptr<InnerBase> inner_;
};
}  // namespace renderer

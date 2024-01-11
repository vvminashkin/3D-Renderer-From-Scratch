#pragma once

#include <Eigen/Dense>
#include <memory>
#include "Mesh.h"

namespace renderer {
    class RGB {
    public:
        double &R() { return val.x(); }

        double &G() { return val.y(); }

        double &B() { return val.z(); }

        //r,g,b
        Eigen::Vector3d val;
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
        AnyObject() = default;

        template<typename T>
        AnyObject(const T &val) {
            inner_ = std::make_unique<Inner < T>>
            (val);
        }

        AnyObject(AnyObject &&) = default;

        AnyObject(const AnyObject &val) {
            inner_ = val.inner_->Clone();
        }

        template<typename T>
        AnyObject &operator=(const T &val) {
            inner_ = std::make_unique<Inner < T>>
            (val);
            return *this;
        }

        AnyObject &operator=(AnyObject &&) = default;

        AnyObject &operator=(const AnyObject &val) {
            AnyObject temp(val);
            std::swap(temp.inner_, this->inner_);
            return *this;
        }

        const Mesh &GetMesh() const{
            return inner_->GetMesh();
        }

    private:
        struct InnerBase {
            virtual ~InnerBase() {}

            virtual std::unique_ptr<InnerBase> Clone() const = 0;
            virtual const Mesh &GetMesh() const = 0;
        };

        template<typename T>
        struct Inner : InnerBase {
            Inner(const T &var) {
                *value_ = var;
            }

            std::unique_ptr<InnerBase> Clone() const override {
                return std::make_unique<Inner>(*value_);
            }
            const Mesh& GetMesh() const override{
                return value_->GetMesh();
            }

        private:
            std::unique_ptr<T> value_;
        };

        std::unique_ptr<InnerBase> inner_;

    };
};
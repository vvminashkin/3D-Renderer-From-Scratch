#include <memory>

#include "Mesh.h"
namespace renderer {
class AnyConstHolderPointer {
    using Vector3d = Eigen::Vector3d;
    using Quaterniond = Eigen::Quaterniond;

public:
    class InnerBase;
    AnyConstHolderPointer() = default;

    AnyConstHolderPointer(const AnyConstHolderPointer &other)
        : inner_((other.inner_ ? other.inner_->Clone() : nullptr)) {
    }
    template <typename T>
    AnyConstHolderPointer(T &&object)
        : inner_(std::make_unique<Inner<std::remove_reference_t<T>>>(std::forward<T>(object))) {
    }
    AnyConstHolderPointer(AnyConstHolderPointer &&) noexcept = default;
    AnyConstHolderPointer &operator=(AnyConstHolderPointer &&) noexcept = default;

    AnyConstHolderPointer &operator=(const AnyConstHolderPointer &val) {
        return *this = AnyConstHolderPointer(val);
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
        using MeshConstIterator = std::vector<Mesh>::const_iterator;
        virtual const Vector3d &GetCoordinates() const = 0;
        virtual const Quaterniond &GetAngle() const = 0;

        virtual Vector3d GetDirection() const = 0;
        virtual Vector3d GetNormal() const = 0;
        virtual Vector3d GetDefaultDirection() const = 0;
        virtual Vector3d GetDefaultNormal() const = 0;
        friend class AnyConstHolderPointer;

    private:
        virtual std::unique_ptr<InnerBase> Clone() const = 0;
    };

private:
    template <typename T>
    class Inner : public InnerBase {
    public:
        const Vector3d &GetCoordinates() const override {
            return value_->GetCoordinates();
        }
        const Quaterniond &GetAngle() const override {
            return value_->GetAngle();
        }

        Vector3d GetDirection() const override {
            return value_->GetDirection();
        };
        Vector3d GetNormal() const override {
            return value_->GetNormal();
        };
        Vector3d GetDefaultDirection() const override {
            return value_->GetDefaultDirection();
        }
        Vector3d GetDefaultNormal() const override {
            return value_->GetDefaultNormal();
        }
        Inner(T &&value) : value_(std::move(value)) {
        }
        Inner(const T &value) : value_(value) {
        }

    private:
        std::unique_ptr<InnerBase> Clone() const override {
            return std::make_unique<Inner>(value_);
        }
        T value_;
    };

    std::unique_ptr<InnerBase> inner_;
};
class AnyHolderPointer {
    using Vector3d = Eigen::Vector3d;
    using Quaterniond = Eigen::Quaterniond;

public:
    class InnerBase;
    AnyHolderPointer() = default;

    AnyHolderPointer(const AnyHolderPointer &other)
        : inner_((other.inner_ ? other.inner_->Clone() : nullptr)) {
    }
    template <typename T>
    AnyHolderPointer(T *&&object)
        : inner_(std::make_unique<Inner<std::remove_cv_t<std::remove_reference_t<T>> *>>(
              std::forward<T *>(object))) {
    }
    AnyHolderPointer(AnyHolderPointer &&) noexcept = default;
    AnyHolderPointer &operator=(AnyHolderPointer &&) noexcept = default;

    AnyHolderPointer &operator=(const AnyHolderPointer &val) {
        return *this = AnyHolderPointer(val);
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
        using MeshConstIterator = std::vector<Mesh>::const_iterator;
        virtual const Vector3d &GetCoordinates() const = 0;
        virtual const Quaterniond &GetAngle() const = 0;

        virtual void SetCoordinates(const Vector3d &) = 0;
        virtual void SetAngle(const Quaterniond &) = 0;

        virtual Vector3d GetDirection() const = 0;
        virtual Vector3d GetNormal() const = 0;
        virtual Vector3d GetDefaultDirection() const = 0;
        virtual Vector3d GetDefaultNormal() const = 0;
        friend class AnyHolderPointer;

    private:
        virtual std::unique_ptr<InnerBase> Clone() const = 0;
    };

private:
    template <typename T>
    class Inner : public InnerBase {
    public:
        const Vector3d &GetCoordinates() const override {
            return value_->GetCoordinates();
        }
        const Quaterniond &GetAngle() const override {
            return value_->GetAngle();
        }
        void SetCoordinates(const Vector3d &vec) override {
            value_->SetCoordinates(vec);
        };
        void SetAngle(const Quaterniond &angle) override {
            value_->SetAngle(angle);
        };

        Vector3d GetDirection() const override {
            return value_->GetDirection();
        };
        Vector3d GetNormal() const override {
            return value_->GetNormal();
        };
        Vector3d GetDefaultDirection() const override {
            return value_->GetDefaultDirection();
        }
        Vector3d GetDefaultNormal() const override {
            return value_->GetDefaultNormal();
        }
        Inner(T &&value) : value_(std::move(value)) {
        }
        Inner(const T &value) : value_(value) {
        }

    private:
        std::unique_ptr<InnerBase> Clone() const override {
            return std::make_unique<Inner>(value_);
        }
        T value_;
    };

    std::unique_ptr<InnerBase> inner_;
};
}  // namespace renderer

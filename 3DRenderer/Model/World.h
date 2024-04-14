#pragma once

#include <vector>
#include "AnyObject.h"
#include "Camera.h"
#include "Eigen/src/Core/Matrix.h"
#include "Light/AmbientLight.h"
#include "Light/PointLight.h"
#include <iostream>

namespace renderer {
class World {
public:
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;
    using Quaterniond = Eigen::Quaterniond;
    template <typename T>
    class TLightHolder;
    World(int width, int height);
    auto GetObjectsIterable() const {
        return Iterable(objects_.begin(), objects_.end());
    };

    void AddObject(AnyObject);
    void AddObject(AnyObject, const Vector3d &);
    void AddAmbientLight();
    void AddPointLight(const Vector3d &);
    const Quaterniond &GetCameraRotation() const;
    const Vector3d &GetCameraPosition() const;
    const Camera &GetCamera() const;
    const std::vector<AmbientLightSource> &GetAmbientLightSources() const;
    const std::vector<TLightHolder<PointLightSource>> &GetPointLightSources() const;

    void SetCameraRotation(const Quaterniond &);
    void SetCameraPosition(const Vector3d &);

    class CameraHolder : public Camera {
    public:
        CameraHolder(const Camera &);
        const Vector3d &GetCoordinates() const;
        const Quaterniond &GetAngle() const;

        void SetCoordinates(const Vector3d &);
        void SetAngle(const Quaterniond &);

        static const Vector3d kDefaultDirection;
        static const Vector3d kDefaultNormal;

    private:
        Eigen::Vector3d coordinates_ = World::GetOrigin();
        Eigen::Quaterniond rotation_;
    };

    class ObjectHolder : public AnyObject {
    public:
        ObjectHolder(AnyObject &&, const Vector3d &, const Eigen::AngleAxisd &);
        ObjectHolder(const AnyObject &);
        ObjectHolder(AnyObject &&) noexcept;
        const Vector3d &GetCoordinates() const;
        const Quaterniond &GetAngle() const;

        void SetCoordinates(const Vector3d &);
        void SetAngle(const Quaterniond &);
        void SetAngle(const Eigen::AngleAxis<double> &);

    private:
        Eigen::Vector3d coordinates_ = World::GetOrigin();
        Eigen::Quaterniond rotation_;
    };

    template <typename T>
    class TLightHolder : public T {
    public:
        TLightHolder(const Vector3d &coordinates) : coordinates_(coordinates) {
        }
        TLightHolder(T &&obj, const Vector3d &coordinates, const Eigen::AngleAxisd &angle)
            : T(obj), coordinates_(coordinates), rotation_(angle) {
        }
        TLightHolder(const T &obj) : T(obj) {
        }
        TLightHolder(T &&obj) noexcept : T(obj) {
        }
        Eigen::Matrix3d GetSmallTriangle() const {
            Eigen::Matrix3d ans;
            ans << coordinates_(0) + 0.05, coordinates_(1) + 0.05, coordinates_(2),
                coordinates_(0) - 0.05, coordinates_(1) + 0.05, coordinates_(2), coordinates_(0),
                coordinates_(1) - 0.05, coordinates_(2);
            std::cout << ans << std::endl;
            return ans;
        }
        const Vector3d &GetCoordinates() const {
            return coordinates_;
        }
        const Quaterniond &GetAngle() const {
            return rotation_;
        }

        void SetCoordinates(const Vector3d &coordinates) {
            coordinates_ = coordinates;
        }
        void SetAngle(const Quaterniond &rotation) {
            rotation_ = rotation;
        }
        void SetAngle(const Eigen::AngleAxis<double> &rotation) {
            rotation_ = rotation;
        }

    private:
        Eigen::Vector3d coordinates_ = World::GetOrigin();
        Eigen::Quaterniond rotation_ = Quaterniond::Identity();
    };
    static Vector3d GetOrigin();

private:
    std::vector<AmbientLightSource> ambient_lights_;
    std::vector<TLightHolder<PointLightSource>> point_lights_;

    std::vector<ObjectHolder> objects_;
    std::vector<CameraHolder> cameras_;
    size_t current_camera_ind_ = 0;
};
}  // namespace renderer

#pragma once

#include <vector>
#include "AnyObject.h"
#include "Camera.h"
#include "Eigen/src/Core/Matrix.h"
#include "Light/AmbientLight.h"
#include "Light/PointLight.h"
#include "AnyHolderPointer.h"
#include "Object.h"
#include <iostream>

namespace renderer {
class World {
public:
    using Vector3d = Eigen::Vector3d;
    using Vector4d = Eigen::Vector4d;
    using Quaterniond = Eigen::Quaterniond;
    template <typename T>
    class TLightHolder;
    class CameraHolder;
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
    CameraHolder &GetCameraHolder();
    const std::vector<AmbientLightSource> &GetAmbientLightSources() const;
    const std::vector<TLightHolder<PointLightSource>> &GetPointLightSources() const;
    std::vector<World::TLightHolder<PointLightSource>> &GetPointLightSources();
    void SetCameraRotation(const Quaterniond &);
    void SetCameraPosition(const Vector3d &);

    class CameraHolder : public Camera {
    public:
        CameraHolder(const Camera &);
        const Vector3d &GetCoordinates() const;
        const Quaterniond &GetAngle() const;

        void SetCoordinates(const Vector3d &);
        void SetAngle(const Quaterniond &);

        Vector3d GetDirection();
        Vector3d GetNormal();
        Vector3d GetDefaultDirection() const {
            return {0, 0, -1};
        }
        Vector3d GetDefaultNormal() const {
            return {0, 1, 0};
        }

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

        Vector3d GetDirection() const;
        Vector3d GetNormal() const;
        Vector3d GetDefaultDirection() const {
            return {0, 0, -1};
        }
        Vector3d GetDefaultNormal() const {
            return {0, 1, 0};
        }

    private:
        Eigen::Vector3d coordinates_ = World::GetOrigin();
        Eigen::Quaterniond rotation_;
    };

    template <typename T>
    class TLightHolder : public T {
    public:
        TLightHolder(const Vector3d &coordinates) : coordinates_(coordinates) {
            representing_triangle_.AddMesh({0, 10, 10}, {1, 1, 1}, {1, 1, 1});
            representing_triangle_.AddTriangle(GetSmallTriangle());
        }
        TLightHolder(T &&obj, const Vector3d &coordinates, const Eigen::AngleAxisd &angle)
            : T(obj), coordinates_(coordinates), rotation_(angle) {
            representing_triangle_.AddMesh({0, 10, 10}, {1, 1, 1}, {1, 1, 1});
            representing_triangle_.AddTriangle(GetSmallTriangle());
        }
        TLightHolder(const T &obj) : T(obj) {
        }
        TLightHolder(T &&obj) noexcept : T(obj) {
        }
        const Mesh &GetRepresentingMesh() const {
            return *(representing_triangle_.GetMeshes().begin());
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
        Vector3d GetDirection() const {
            return {0, 0, -1};
        }
        Vector3d GetNormal() const {
            return {0, 1, 0};
        }
        Vector3d GetDefaultDirection() const {
            return {0, 0, -1};
        }
        Vector3d GetDefaultNormal() const {
            return {0, 1, 0};
        }

    private:
        using ColorFunction = const std::function<RGB(const Triangle &, const Vector3d &)>;
        Eigen::Matrix3d GetSmallTriangle() const {
            Eigen::Matrix3d ans;
            ans << 0.05, 0.05, 0, -0.05, 0.05, 0, 0, -0.05, 0;
            return ans;
        }

        Eigen::Vector3d coordinates_ = World::GetOrigin();
        Eigen::Quaterniond rotation_ = Quaterniond::Identity();
        BasicObject representing_triangle_;
    };
    static Vector3d GetOrigin();
    std::vector<ObjectHolder> &GetObjects();
    std::vector<TLightHolder<PointLightSource>> &GetLights();

private:
    std::vector<AmbientLightSource> ambient_lights_;
    std::vector<TLightHolder<PointLightSource>> point_lights_;

    std::vector<ObjectHolder> objects_;
    std::vector<CameraHolder> cameras_;
    size_t current_camera_ind_ = 0;
};
}  // namespace renderer

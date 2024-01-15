#pragma once

#include <vector>
#include "Objects.h"
#include "Camera.h"

namespace renderer {
class World {
public:
    auto GetObjectsIterable() const;

    void AddObject(AnyObject);

    void AddObject(AnyObject, const Vector3d &);

    const Quaterniond &GetCameraRotation() const;

    const Vector3d GetCameraPosition() const;

    const Camera& GetCamera()const;

    class CameraHolder : public Camera {
    public:
        CameraHolder(const Camera &);
        const Vector3d &GetCoordinates() const;
        const Quaterniond &GetAngle() const;

        void SetCoordinates(const Vector3d &);
        void SetAngle(const Quaterniond &);

    private:
        Eigen::Vector3d coordinates_ = World::GetOrigin();
        Eigen::Quaterniond rotation_;
    };

    class ObjectHolder : public AnyObject {
    public:
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

private:
    static Vector3d GetOrigin();
    std::vector<ObjectHolder> objects_;
    std::vector<CameraHolder> cameras_;
    size_t current_camera_ind_;
};
}  // namespace renderer

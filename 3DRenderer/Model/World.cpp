#include "World.h"

namespace renderer {

void World::AddObject(AnyObject object) {
    objects_.emplace_back(std::move(object));

    objects_.back().SetAngle(Eigen::AngleAxis<double>(0, Eigen::Vector3d::UnitX()));
}
void World::AddObject(AnyObject object, const Eigen::Vector3d &global_coordinates) {
    objects_.emplace_back(std::move(object));
    objects_.back().SetCoordinates(global_coordinates);
    objects_.back().SetAngle(Eigen::AngleAxis<double>(0, Eigen::Vector3d::UnitX()));
}
Vector3d World::GetOrigin() {
    return {0, 0, 0};
}
const Quaterniond &World::GetCameraRotation() const {
    return cameras_[current_camera_ind_].GetAngle();
}
const Vector3d World::GetCameraPosition() const {
    return cameras_[current_camera_ind_].GetCoordinates();
}
const Camera &World::GetCamera() const {
    return cameras_[current_camera_ind_];
}

World::CameraHolder::CameraHolder(const Camera &cam) : Camera(cam) {
}
const Vector3d &World::CameraHolder::GetCoordinates() const {
    return coordinates_;
}
const Quaterniond &World::CameraHolder::GetAngle() const {
    return rotation_;
}
void World::CameraHolder::SetCoordinates(const Vector3d &coordinates) {
    coordinates_ = coordinates;
}
void World::CameraHolder::SetAngle(const Quaterniond &angle) {
    rotation_ = angle;
}
World::ObjectHolder::ObjectHolder(const AnyObject &obj) : AnyObject(obj) {
}
World::ObjectHolder::ObjectHolder(AnyObject &&obj) noexcept
    : AnyObject(std::forward<AnyObject>(obj)) {
}
const Vector3d &World::ObjectHolder::GetCoordinates() const {
    return coordinates_;
}
const Quaterniond &World::ObjectHolder::GetAngle() const {
    return rotation_;
}
void World::ObjectHolder::SetCoordinates(const Vector3d &coordinates) {
    coordinates_ = coordinates;
}
void World::ObjectHolder::SetAngle(const Quaterniond &angle) {
    rotation_ = angle;
}
void World::ObjectHolder::SetAngle(const Eigen::AngleAxis<double> &angle) {
    rotation_ = angle;
}
}  // namespace renderer

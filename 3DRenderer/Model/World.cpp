#include "World.h"
#include <iostream>

namespace renderer {
World::World(int width, int height) {
    Camera curr(width, height);
    cameras_.push_back(curr);
}
void World::AddObject(AnyObject object) {
    objects_.emplace_back(std::move(object), World::GetOrigin(),
                          Eigen::AngleAxis<double>(0, Eigen::Vector3d::UnitX()));
}
void World::AddObject(AnyObject object, const Eigen::Vector3d &global_coordinates) {
    objects_.emplace_back(std::move(object), global_coordinates,
                          Eigen::AngleAxis<double>(0, Eigen::Vector3d::UnitX()));
}
void World::AddAmbientLight() {
    ambient_lights_.emplace_back();
}
void World::AddPointLight(const Vector3d &coordinates) {
    point_lights_.emplace_back(coordinates);
}
World::Vector3d World::GetOrigin() {
    return {0, 0, 0};
}
const World::Quaterniond &World::GetCameraRotation() const {
    return cameras_[current_camera_ind_].GetAngle();
}
const World::Vector3d &World::GetCameraPosition() const {
    return cameras_[current_camera_ind_].GetCoordinates();
}
const Camera &World::GetCamera() const {
    return cameras_[current_camera_ind_];
}
const std::vector<AmbientLightSource> &World::GetAmbientLightSources() const {
    return ambient_lights_;
}
const std::vector<World::TLightHolder<PointLightSource>> &World::GetPointLightSources() const {
    return point_lights_;
}

std::vector<World::TLightHolder<PointLightSource>> &World::GetPointLightSources() {
    return point_lights_;
}
void World::SetCameraRotation(const Quaterniond &rotation) {
    cameras_[current_camera_ind_].SetAngle(rotation);
}
void World::SetCameraPosition(const Vector3d &position) {
    cameras_[current_camera_ind_].SetCoordinates(position);
}
const World::Vector3d &World::CameraHolder::GetCoordinates() const {
    return coordinates_;
}
const World::Quaterniond &World::CameraHolder::GetAngle() const {
    return rotation_;
}
void World::CameraHolder::SetCoordinates(const Vector3d &coordinates) {

    coordinates_ = coordinates;
}
void World::CameraHolder::SetAngle(const Quaterniond &angle) {
    rotation_ = angle;
}
World::Vector3d World::CameraHolder::GetDirection() {
    return rotation_ * Vector3d{0, 0, -1};
};
World::Vector3d World::CameraHolder::GetNormal() {
    return rotation_ * Vector3d{0, 1, 0};
};

World::ObjectHolder::ObjectHolder(AnyObject &&obj, const Vector3d &coord,
                                  const Eigen::AngleAxisd &rotation)
    : AnyObject(std::move(obj)), coordinates_(coord), rotation_(rotation) {
}
World::ObjectHolder::ObjectHolder(const AnyObject &obj) : AnyObject(obj) {
}
World::ObjectHolder::ObjectHolder(AnyObject &&obj) noexcept : AnyObject(std::move(obj)) {
}
const World::Vector3d &World::ObjectHolder::GetCoordinates() const {
    return coordinates_;
}
const World::Quaterniond &World::ObjectHolder::GetAngle() const {
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
World::CameraHolder::CameraHolder(const Camera &camera) : Camera(camera) {
}

std::vector<World::ObjectHolder> &World::GetObjects() {
    return objects_;
}
std::vector<World::TLightHolder<PointLightSource>> &World::GetLights() {
    return point_lights_;
}
World::CameraHolder &World::GetCameraHolder() {
    return cameras_[current_camera_ind_];
};
World::Vector3d World::ObjectHolder::GetDirection() const {
    return {0, 0, -1};
}
World::Vector3d World::ObjectHolder::GetNormal() const {
    return {0, 1, 0};
}
}  // namespace renderer

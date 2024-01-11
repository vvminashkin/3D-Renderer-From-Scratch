#include "World.h"
namespace renderer {
    const std::vector<World::ObjectContainer> &World::GetObjects() {
        return objects_;
    }

    void World::AddObject(AnyObject object) {
        objects_.emplace_back();
        objects_.back().object = std::move(object);

        objects_.back().global_rotation_  = Eigen::AngleAxis<double>(0,Eigen::Vector3d::UnitX());

    }

    void World::AddObject(AnyObject object, Eigen::Vector3d  global_coordinates) {
        objects_.emplace_back();
        objects_.back().object = std::move(object);
        objects_.back().global_coordinates = std::move(global_coordinates);
        objects_.back().global_rotation_  = Eigen::AngleAxis<double>(0,Eigen::Vector3d::UnitX());

    }

    const Eigen::Quaterniond &World::GetCameraRotation() {
        return camera_rotation_;
    }

    const Eigen::Vector3d World::GetCameraPosition() {
        return camera_coordinates_;
    }
}
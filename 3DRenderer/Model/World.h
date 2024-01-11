#pragma once

#include <vector>
#include "Objects.h"
#include "Camera.h"

namespace renderer {
    class World {
    public:
        struct ObjectContainer;
        const std::vector<ObjectContainer> &GetObjects();

        void AddObject(AnyObject object);

        void AddObject(AnyObject object, Eigen::Vector3d);

        const Eigen::Quaterniond& GetCameraRotation();
        const Eigen::Vector3d GetCameraPosition();
        struct ObjectContainer {
            AnyObject object;
            Eigen::Vector3d global_coordinates = {0, 0, 0};
            Eigen::Quaterniond global_rotation_ ;
        };

    private:
        std::vector<ObjectContainer> objects_;
        Camera camera_;
        Eigen::Vector3d camera_coordinates_ = {0,0,0};
        Eigen::Quaterniond camera_rotation_;
    };
}



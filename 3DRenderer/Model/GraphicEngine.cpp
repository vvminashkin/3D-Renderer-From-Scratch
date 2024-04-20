#include "GraphicEngine.h"
#include "Eigen/src/Core/Matrix.h"
#include "Primitives.h"
#include "Screen.h"
#include "Utils.h"
#include "Object.h"
#include "ObjLoader/ObjLoader.h"
#include <algorithm>
#include <cmath>
#include <memory>

namespace kernel {
GraphicEngine::GraphicEngine(int width, int height)
    : width_(width),
      height_(height),
      current_screen_(width, height),
      update_port_([this]() -> PortReturnType { return this->current_screen_; }),
      world_(width, height) {

    renderer::BasicObject object = ReadObject("suzanne.obj");
    world_.AddObject(object);
    world_.AddAmbientLight();
    world_.AddPointLight({0.4, 4.3, 2.9});
    // world_.AddPointLight({-0.3, 1, 0});
    renderer::BasicObject objectlight;
    objectlight.AddMesh({0, 10, 10}, {1, 1, 1}, {1, 1, 1});
    objectlight.AddTriangle(world_.GetPointLightSources()[0].GetSmallTriangle());
    world_.AddObject(objectlight);
}

const renderer::Screen& GraphicEngine::GetCurrentScreen() {
    return current_screen_;
}

void GraphicEngine::Subscribe(observer::CObserver<const renderer::Screen>* obs) {
    update_port_.subscribe(obs);
}
void GraphicEngine::TestUpdateProjection() {
    Screen screen(*renderer_.Draw(world_, width_, height_));
    current_screen_ = std::move(screen);
    update_port_.notify();
}
void GraphicEngine::TiltCameraUp(double shift) {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Quaterniond new_quatenion = Quaterniond(camera_rotation.matrix() *
                                            Eigen::AngleAxisd(shift, Vector3d{1, 0, 0}).matrix());
    new_quatenion.normalize();
    world_.SetCameraRotation(new_quatenion);
}
void GraphicEngine::TiltCameraDown(double shift) {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Vector3d direction = camera_rotation * World::CameraHolder::kDefaultDirection;
    Vector3d normal = camera_rotation * World::CameraHolder::kDefaultNormal;
    Vector3d axis_of_rotation = direction.cross(normal);
    Quaterniond new_quatenion = Quaterniond(camera_rotation.matrix() *
                                            Eigen::AngleAxisd(-shift, Vector3d{1, 0, 0}).matrix());
    new_quatenion.normalize();
    world_.SetCameraRotation(new_quatenion);
}
void GraphicEngine::TiltCameraRight(double shift) {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Quaterniond new_quatenion =
        Quaterniond(camera_rotation.matrix() *
                    Eigen::AngleAxisd(-shift, World::CameraHolder::kDefaultNormal).matrix());
    new_quatenion.normalize();
    world_.SetCameraRotation(new_quatenion);
}
void GraphicEngine::TiltCameraLeft(double shift) {

    Quaterniond camera_rotation = world_.GetCameraRotation();
    Quaterniond new_quatenion =
        Quaterniond(camera_rotation.matrix() *
                    Eigen::AngleAxisd(shift, World::CameraHolder::kDefaultNormal).matrix());
    new_quatenion.normalize();
    world_.SetCameraRotation(new_quatenion);
}
void GraphicEngine::MoveCameraUp(double shift) {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Vector3d direction = camera_rotation * World::CameraHolder::kDefaultDirection;
    Vector3d normal = camera_rotation * World::CameraHolder::kDefaultNormal;
    world_.SetCameraPosition(world_.GetCameraPosition() + normal * shift);
}
void GraphicEngine::MoveCameraDown(double shift) {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Vector3d direction = camera_rotation * World::CameraHolder::kDefaultDirection;
    Vector3d normal = camera_rotation * World::CameraHolder::kDefaultNormal;
    world_.SetCameraPosition(world_.GetCameraPosition() - normal * shift);
}
void GraphicEngine::MoveCameraRight(double shift) {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Vector3d direction = camera_rotation * World::CameraHolder::kDefaultDirection;
    Vector3d normal = camera_rotation * World::CameraHolder::kDefaultNormal;
    Vector3d axis_of_rotation = direction.cross(normal);
    world_.SetCameraPosition(world_.GetCameraPosition() + axis_of_rotation * shift);
}
void GraphicEngine::MoveCameraLeft(double shift) {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Vector3d direction = camera_rotation * World::CameraHolder::kDefaultDirection;
    Vector3d normal = camera_rotation * World::CameraHolder::kDefaultNormal;
    Vector3d axis_of_rotation = direction.cross(normal);
    world_.SetCameraPosition(world_.GetCameraPosition() - axis_of_rotation * shift);
}
void GraphicEngine::MoveCameraForward(double shift) {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Vector3d direction = camera_rotation * World::CameraHolder::kDefaultDirection;
    Vector3d normal = camera_rotation * World::CameraHolder::kDefaultNormal;
    world_.SetCameraPosition(world_.GetCameraPosition() + direction * shift);
}
void GraphicEngine::MoveCameraBackward(double shift) {
    Quaterniond camera_rotation = world_.GetCameraRotation();
    Vector3d direction = camera_rotation * World::CameraHolder::kDefaultDirection;
    Vector3d normal = camera_rotation * World::CameraHolder::kDefaultNormal;
    world_.SetCameraPosition(world_.GetCameraPosition() - direction * shift);
}
void GraphicEngine::SwitchLightingModel() {
    renderer_.SwitchLightModel();
}
}  // namespace kernel

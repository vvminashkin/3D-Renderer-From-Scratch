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
    selected_ = &world_.GetCameraHolder();
    ReadAllFromDirectory("Models", &world_);
    world_.AddAmbientLight();
    world_.AddPointLight({0, 0, 4});
    world_.SetCameraPosition({0, 0, 4});

    world_.AddObject(renderer::Sphere{{1, 1, 1}, {1, 1, 1}, {1, 1, 1}});
    // world_.AddPointLight({0.4, 4.3, 60});
    // world_.SetCameraPosition({0,0,80});
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
GraphicEngine::AnyHolderPointer GraphicEngine::GetSelected() {
    return selected_;
    ;
}
void GraphicEngine::TiltUp(double shift, AnyHolderPointer selected) {
    Quaterniond rotation = selected->GetAngle();
    Quaterniond new_quatenion =
        Quaterniond(rotation.matrix() * Eigen::AngleAxisd(shift, Vector3d{1, 0, 0}).matrix());
    new_quatenion.normalize();
    selected->SetAngle(new_quatenion);
}
void GraphicEngine::TiltDown(double shift, AnyHolderPointer selected) {
    Quaterniond rotation = selected->GetAngle();
    Quaterniond new_quatenion =
        Quaterniond(rotation.matrix() * Eigen::AngleAxisd(-shift, Vector3d{1, 0, 0}).matrix());
    new_quatenion.normalize();
    selected->SetAngle(new_quatenion);
}
void GraphicEngine::TiltRight(double shift, AnyHolderPointer selected) {
    Quaterniond rotation = selected->GetAngle();
    Quaterniond new_quatenion = Quaterniond(
        rotation.matrix() * Eigen::AngleAxisd(-shift, selected->GetDefaultNormal()).matrix());
    new_quatenion.normalize();
    selected->SetAngle(new_quatenion);
}
void GraphicEngine::TiltLeft(double shift, AnyHolderPointer selected) {

    Quaterniond rotation = selected->GetAngle();
    Quaterniond new_quatenion = Quaterniond(
        rotation.matrix() * Eigen::AngleAxisd(shift, selected->GetDefaultNormal()).matrix());
    new_quatenion.normalize();
    selected->SetAngle(new_quatenion);
}

void GraphicEngine::MoveUp(double shift, AnyHolderPointer selected) {

    Quaterniond rotation = selected->GetAngle();
    Vector3d normal = selected->GetNormal();
    selected->SetCoordinates(selected->GetCoordinates() + normal * shift);
}
void GraphicEngine::MoveDown(double shift, AnyHolderPointer selected) {
    Quaterniond rotation = selected->GetAngle();
    Vector3d normal = selected->GetNormal();
    selected->SetCoordinates(selected->GetCoordinates() - normal * shift);
}
void GraphicEngine::MoveRight(double shift, AnyHolderPointer selected) {
    Quaterniond rotation = selected->GetAngle();
    Vector3d direction = selected->GetDirection();
    Vector3d normal = selected->GetNormal();
    Vector3d axis_of_rotation = direction.cross(normal);
    selected->SetCoordinates(selected->GetCoordinates() + axis_of_rotation * shift);
}
void GraphicEngine::MoveLeft(double shift, AnyHolderPointer selected) {
    Quaterniond rotation = selected->GetAngle();
    Vector3d direction = selected->GetDirection();
    Vector3d normal = selected->GetNormal();
    Vector3d axis_of_rotation = direction.cross(normal);
    selected->SetCoordinates(selected->GetCoordinates() - axis_of_rotation * shift);
}
void GraphicEngine::MoveForward(double shift, AnyHolderPointer selected) {
    Quaterniond rotation = selected->GetAngle();
    Vector3d direction = selected->GetDirection();
    selected->SetCoordinates(selected->GetCoordinates() + direction * shift);
}
void GraphicEngine::MoveBackward(double shift, AnyHolderPointer selected) {
    Quaterniond rotation = selected->GetAngle();
    Vector3d direction = selected->GetDirection();
    selected->SetCoordinates(selected->GetCoordinates() - direction * shift);
}
void GraphicEngine::SwitchLightingModel() {
    renderer_.SwitchLightModel();
}
void GraphicEngine::SwitchObject() {
    if (world_.GetObjects().empty()) {
        return;
    }
    if (is_changing_objects_) {
        ++selected_obj_;
        if (selected_obj_ == world_.GetObjects().size()) {
            selected_obj_ = 0;
        }
    } else {
        ResetEditingStates();
        is_changing_objects_ = true;
    }
    selected_ = &world_.GetObjects()[selected_obj_];
}
void GraphicEngine::SwitchCamera() {
    ResetEditingStates();
    is_changing_camera_ = true;
    selected_ = &world_.GetCameraHolder();
}
void GraphicEngine::SwitchLight() {
    if (world_.GetPointLightSources().empty()) {
        return;
    }
    if (is_changing_point_lights_) {
        ++selected_point_light_;
        if (selected_point_light_ == world_.GetPointLightSources().size()) {
            selected_point_light_ = 0;
        }
    } else {
        ResetEditingStates();
        is_changing_point_lights_ = true;
    }
    selected_ = &world_.GetPointLightSources()[selected_point_light_];
}
void GraphicEngine::ResetEditingStates() {
    is_changing_objects_ = false;
    is_changing_camera_ = false;
    is_changing_point_lights_ = false;
}
void GraphicEngine::AddPointLight() {
    world_.AddPointLight(world_.GetCameraPosition());
}
void GraphicEngine::ChangeSelectedLight(double c_attenuation, double l_attenuation,
                                        double q_attenuation) {
    auto& lights = world_.GetLights();
    lights[selected_point_light_].SetParameters(c_attenuation, l_attenuation, q_attenuation);
}
void GraphicEngine::ToggleSelectedLight() {
    auto& lights = world_.GetLights();
    lights[selected_point_light_].Toggle();
}
}  // namespace kernel

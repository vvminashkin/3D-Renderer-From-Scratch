#pragma once
#include "Renderer.h"
#include "../Observer/Observer.h"
#include "Screen.h"
#include "Utils.h"

namespace kernel {
class GraphicEngine {
public:
    using World = renderer::World;
    using Screen = renderer::Screen;
    using Quaterniond = Eigen::Quaterniond;
    using Vector3d = Eigen::Vector3d;
    using RGB = renderer::RGB;
    using AnyHolderPointer = renderer::AnyHolderPointer;
    GraphicEngine(int width, int height);
    AnyHolderPointer GetSelected();
    void TiltUp(double shift, AnyHolderPointer selected);
    void TiltDown(double shift, AnyHolderPointer selected);
    void TiltRight(double shift, AnyHolderPointer selected);
    void TiltLeft(double shift, AnyHolderPointer selected);

    void MoveUp(double shift, AnyHolderPointer selected);
    void MoveDown(double shift, AnyHolderPointer selected);
    void MoveRight(double shift, AnyHolderPointer selected);
    void MoveLeft(double shift, AnyHolderPointer selected);
    void MoveForward(double shift, AnyHolderPointer selected);
    void MoveBackward(double shift, AnyHolderPointer selected);

    void Subscribe(observer::CObserver<const Screen>* obs);
    void TestUpdateProjection();
    void SwitchLightingModel();
    void SwitchObject();
    void SwitchCamera();
    void SwitchLight();

private:
    void ResetEditingStates();
    using PortReturnType = observer::CObservable<const Screen>::CReturn;

    const renderer::Screen& GetCurrentScreen();
    renderer::Renderer renderer_;
    World world_;
    observer::CObservable<const Screen> update_port_;
    Screen current_screen_;
    int width_;
    int height_;
    std::vector<renderer::RGB> test_colors_ = {{0.5, 0.5, 0.5}, {0, 0, 0}};
    bool is_changing_objects_ = false;
    bool is_changing_camera_ = true;
    bool is_changing_point_lights_ = false;
    size_t selected_obj_ = 0;
    size_t selected_point_light_ = 0;
    AnyHolderPointer selected_;
};

};  // namespace kernel

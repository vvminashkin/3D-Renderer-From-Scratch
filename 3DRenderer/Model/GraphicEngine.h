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
    GraphicEngine(int width, int height);
    void TiltCameraUp(double shift);
    void TiltCameraDown(double shift);
    void TiltCameraRight(double shift);
    void TiltCameraLeft(double shift);

    void MoveCameraUp(double shift);
    void MoveCameraDown(double shift);
    void MoveCameraRight(double shift);
    void MoveCameraLeft(double shift);
    void MoveCameraForward(double shift);
    void MoveCameraBackward(double shift);

    void Subscribe(observer::CObserver<const Screen>* obs);
    void TestUpdateProjection();

private:
    const renderer::Screen& GetCurrentScreen();
    renderer::Renderer renderer_;
    renderer::World world_;
    observer::CObservable<const Screen> update_port_;
    renderer::Screen current_screen_;
    int width_;
    int height_;
    std::vector<renderer::RGB> test_colors_ = {{0.5, 0.5, 0.5}, {0, 0, 0}};
    int test_current_index_ = 0;
    double test_current_plane_d_ = 1;
};

};  // namespace kernel

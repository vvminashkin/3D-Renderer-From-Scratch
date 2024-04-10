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
    void TiltCameraUp();

    void Subscribe(observer::CObserver<const Screen>* obs);
    void TestUpdateRasterization();
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

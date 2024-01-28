#pragma once
#include <algorithm>
#include <memory>
#include "Renderer.h"
#include "../Observer/Observer.h"
#include "Screen.h"
namespace model {
class Model {
public:
    Model();
    void Subscribe(observer::CObserver<std::shared_ptr<const renderer::Screen>()> *obs);

private:
    std::shared_ptr<const renderer::Screen> GetCurrentScreen();
    renderer::Renderer renderer_;
    renderer::World world_;
    observer::CObservable<std::shared_ptr<const renderer::Screen>()> update_port_;
    std::shared_ptr<const renderer::Screen> current_screen_;
};

};  // namespace model

#pragma once
#include "Model/GraphicEngine.h"
#include "Controller/GEngineController.h"
#include "View/MainScreen.h"

namespace application {
void HandleException();

class Application {
    using GraphicEngine = kernel::GraphicEngine;
    using MainScreen = interface::MainScreen;
    using GEngineController = interface::GEngineController;

public:
    Application();
    void Run();

private:
    static constexpr int kDefaultWidth = 1500;
    static constexpr int kDefaultHeight = 1000;

    GraphicEngine model_;
    MainScreen view_;
    GEngineController controller_;
};
}  // namespace application

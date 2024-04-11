#include "Application.h"
#include "Controller/GEngineController.h"
#include <iostream>
namespace application {
void HandleException() {

    try {
        throw;
    } catch (std::exception& e) {
        std::cerr << "Application failed with exception:" << e.what();

    } catch (...) {
        std::cerr << "Application failed with unknown exception";
    }
}
Application::Application()
    : model_(kDefaultWidth, kDefaultHeight),
      view_(kDefaultWidth, kDefaultHeight),
      controller_(&model_) {
    model_.Subscribe(view_.GetRecievePort());
}
void Application::Run() {
    bool closed = false;
    sf::Event event;
    while (!closed) {
        if (view_.GetEvent(event)) {

            closed = controller_.HandleEvent(event);
            if (closed) {
                break;
            }
        } else {
            controller_.HandleStallEvent();
        }
    }
}

}  // namespace application

#include "Controller.h"
#include <memory>
namespace controller {
Controller::Controller()
    : update_listener_(
          []() {}, [this](std::shared_ptr<const renderer::Screen> screen) { DrawPicture(screen); },
          []() {}){};
Controller::RecievePortType* Controller::GetRecievePort() {
    return &update_listener_;
}
}  // namespace controller

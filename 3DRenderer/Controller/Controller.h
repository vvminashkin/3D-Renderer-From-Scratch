#pragma once
#include "../Observer/Observer.h"
#include "../Model/Screen.h"
#include <memory>
namespace controller {
class Controller {
public:
    using RecievePortType = observer::CObserver<std::shared_ptr<const renderer::Screen>()>;
    Controller();
    RecievePortType* GetRecievePort();

private:
    void DrawPicture(std::shared_ptr<const renderer::Screen> screen) {
    }
    RecievePortType update_listener_;
};

}  // namespace controller

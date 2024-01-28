#pragma once
#include "../Observer/Observer.h"
#include <memory>
#include "../Model/Screen.h"
namespace view {
class View {
public:
    View();

private:
    void Draw(std::shared_ptr<const renderer::Screen>);
    observer::CObserver<std::shared_ptr<const renderer::Screen()>> recieve_port_;
};
}  // namespace view

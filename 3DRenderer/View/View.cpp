#include "View.h"
namespace view {
View::View()
    : recieve_port_([]() {},
                    [this](std::shared_ptr<const renderer::Screen> screen) { Draw(screen); },
                    []() {}) {
}

void View::Draw(std::shared_ptr<const renderer::Screen>) {
}
}  // namespace view

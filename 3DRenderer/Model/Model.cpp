#include "Model.h"
namespace model {
Model::Model() : update_port_([this]() { return GetCurrentScreen(); }) {
}

std::shared_ptr<const renderer::Screen> Model::GetCurrentScreen() {
    return current_screen_;
}

void Model::Subscribe(observer::CObserver<std::shared_ptr<const renderer::Screen>()> *obs) {
    update_port_.subscribe(obs);
}
}  // namespace model

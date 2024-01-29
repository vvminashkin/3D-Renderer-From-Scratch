#include "Controller.h"
#include <SFML/Window/Keyboard.hpp>
#include <memory>
namespace controller {
Controller::Controller() {
}

bool Controller::HandleEvent(const sf::Event& event, model::Model& model) {
    switch (event.type) {
        case sf::Event::Closed:
            return true;
        case sf::Event::KeyPressed:
            switch (event.key.code) {

                case sf::Keyboard::A:
                    model.TestUpdate();
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return false;
}
}  // namespace controller

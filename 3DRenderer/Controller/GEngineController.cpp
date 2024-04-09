#include "GEngineController.h"
#include <SFML/Window/Keyboard.hpp>
#include <memory>
namespace interface {
GEngineController::GEngineController(GraphicEngine* model) : model_(model) {
}

bool GEngineController::HandleEvent(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed:
            return true;
        case sf::Event::KeyPressed:
            HandleKeyEvent(event.key);
            break;
        default:
            break;
    }
    return false;
}
bool GEngineController::HandleKeyEvent(const sf::Event::KeyEvent& event) {

    switch (event.code) {

        case sf::Keyboard::A:
            model_->TestUpdateProjection();
            break;
        default:
            break;
    }
    return false;
}
}  // namespace interface

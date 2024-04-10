#include "GEngineController.h"
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <iostream>

namespace interface {
GEngineController::GEngineController(GraphicEngine* model) : model_(model) {
}

bool GEngineController::HandleEvent(const sf::Event& event) {
    // std::cout<< event.type<<std::endl;
    switch (event.type) {
        case sf::Event::Closed:
            return true;
        case sf::Event::KeyPressed:
            return HandleKeyEvent(event.key);
            break;
        default:
            break;
    }
    return false;
}
bool GEngineController::HandleKeyEvent(const sf::Event::KeyEvent& event) {

    // std::cout<<event.code<<std::endl;
    // TODO holding key
    switch (event.code) {

        case sf::Keyboard::A:
            model_->TestUpdateProjection();
            break;
        case sf::Keyboard::Escape:
            return true;
        case sf::Keyboard::Up:
            model_->TiltCameraUp();
            model_->TestUpdateProjection();
            break;

        default:
            break;
    }
    return false;
}
}  // namespace interface

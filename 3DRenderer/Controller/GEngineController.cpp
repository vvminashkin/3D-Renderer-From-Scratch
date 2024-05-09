#include "GEngineController.h"
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include <iostream>

namespace interface {
GEngineController::GEngineController(GraphicEngine* model) : model_(model) {
}

bool GEngineController::HandleEvent(const sf::Event& event) {
    // std::cout << event.type << std::endl;
    switch (event.type) {
        case sf::Event::Closed:
            return true;
        case sf::Event::KeyPressed:
            return HandleKeyPressedEvent(event.key);
            break;
        case sf::Event::KeyReleased:
            HandleKeyReleasedEvent(event.key);
            break;
        default:
            break;
    }
    return false;
}
void GEngineController::HandleStallEvent() {
    HandleKeys();
}
bool GEngineController::HandleKeyPressedEvent(const sf::Event::KeyEvent& event) {
    switch (event.code) {

        case sf::Keyboard::Escape:
            return true;
        case sf::Keyboard::Up:
            pressed_keys_.push_back(sf::Keyboard::Up);
            break;
        case sf::Keyboard::Down:
            pressed_keys_.push_back(sf::Keyboard::Down);
            break;
        case sf::Keyboard::Right:
            pressed_keys_.push_back(sf::Keyboard::Right);
            break;
        case sf::Keyboard::Left:
            pressed_keys_.push_back(sf::Keyboard::Left);
            break;
        case sf::Keyboard::Space:
            pressed_keys_.push_back(sf::Keyboard::Space);
            break;
        case sf::Keyboard::Z:
            pressed_keys_.push_back(sf::Keyboard::Z);
            break;
        case sf::Keyboard::W:
            pressed_keys_.push_back(sf::Keyboard::W);
            break;
        case sf::Keyboard::S:
            pressed_keys_.push_back(sf::Keyboard::S);
            break;
        case sf::Keyboard::D:
            pressed_keys_.push_back(sf::Keyboard::D);
            break;
        case sf::Keyboard::A:
            pressed_keys_.push_back(sf::Keyboard::A);
            break;
        case sf::Keyboard::L:
            model_->SwitchLightingModel();
            model_->TestUpdateProjection();
            break;
        case sf::Keyboard::O:
            model_->SwitchObject();
            break;
        case sf::Keyboard::C:
            model_->SwitchCamera();
            break;
        case sf::Keyboard::P:
            model_->SwitchLight();
            break;
        case sf::Keyboard::E:
            model_->AddPointLight();
            model_->TestUpdateProjection();
            break;
        case sf::Keyboard::Q:
            double c_attenuation, l_attenuation, q_attenuation;
            std::cout << "Please enter light parameters" << std::endl;
            std::cout << "Constant attenuation:" << std::endl;
            std::cin >> c_attenuation;
            std::cout << "Linear attenuation:" << std::endl;
            std::cin >> l_attenuation;
            std::cout << "Quadratic attenuation:" << std::endl;
            std::cin >> q_attenuation;
            model_->ChangeSelectedLight(c_attenuation, l_attenuation, q_attenuation);
            model_->TestUpdateProjection();
            break;
        case sf::Keyboard::I:
            model_->ToggleSelectedLight();
            model_->TestUpdateProjection();
            break;
        default:
            break;
    }
    return false;
}
void GEngineController::HandleKeyReleasedEvent(const sf::Event::KeyEvent& event) {
    auto it = pressed_keys_.begin();
    while (it != pressed_keys_.end()) {
        if (*it == event.code) {
            pressed_keys_.erase(it);
            break;
        }
        ++it;
    }
}
void GEngineController::HandleKeys() {
    for (const auto& key : pressed_keys_) {
        switch (key) {
            case sf::Keyboard::Up:
                model_->TiltUp(kDefaultRotation, model_->GetSelected());
                break;
            case sf::Keyboard::Down:
                model_->TiltDown(kDefaultRotation, model_->GetSelected());
                break;
            case sf::Keyboard::Right:
                model_->TiltRight(kDefaultRotation, model_->GetSelected());
                break;
            case sf::Keyboard::Left:
                model_->TiltLeft(kDefaultRotation, model_->GetSelected());
                break;
            case sf::Keyboard::Space:
                model_->MoveUp(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::Z:
                model_->MoveDown(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::W:
                model_->MoveForward(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::S:
                model_->MoveBackward(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::D:
                model_->MoveRight(kDefaultMove, model_->GetSelected());
                break;
            case sf::Keyboard::A:
                model_->MoveLeft(kDefaultMove, model_->GetSelected());
                break;
            default:
                break;
        }
    }
    if (!pressed_keys_.empty()) {
        model_->TestUpdateProjection();
    }
}
}  // namespace interface

#pragma once
#include "../Observer/Observer.h"
#include "../Model/Screen.h"
#include "../Model/GraphicEngine.h"
#include <SFML/Graphics.hpp>
#include <memory>
namespace interface {
class GEngineController {
    using GraphicEngine = kernel::GraphicEngine;

public:
    GEngineController(GraphicEngine* model);
    bool HandleEvent(const sf::Event&);

    void InitEnviroment();
    void HandleStallEvent();

private:
    bool HandleKeyPressedEvent(const sf::Event::KeyEvent&);
    void HandleKeyReleasedEvent(const sf::Event::KeyEvent&);
    void HandleKeys();
    static constexpr double kDefaultRotation = 0.02;
    static constexpr double kDefaultMove = 0.1;
    std::list<sf::Keyboard::Key> pressed_keys_;
    GraphicEngine* model_;
};

}  // namespace interface

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

private:
    bool HandleKeyEvent(const sf::Event::KeyEvent&);
    GraphicEngine* model_;
};

}  // namespace interface

#pragma once
#include "../Observer/Observer.h"
#include <memory>
#include "../Model/Screen.h"
#include <SFML/Graphics.hpp>
namespace interface {
class MainScreen {
    static constexpr uint8_t kOpaque = 255;
    static constexpr uint8_t kTransparent = 255;
    struct RGBA {
        uint8_t R, G, B;
        uint8_t A = kOpaque;
        static RGBA FromRGB(renderer::RGB color);
    };

public:
    using RecievePortType = observer::CObserver<const renderer::Screen>;
    MainScreen(int width, int height);
    bool GetEvent(sf::Event &event);
    RecievePortType *GetRecievePort();

private:
    static RGBA DRGBTo8BitRGBA(const renderer::RGB &color);
    void Draw(const renderer::Screen &);
    sf::Image MakeImage(const renderer::Screen &matrix);

    RecievePortType recieve_port_;
    sf::RenderWindow window_;
    sf::Texture render_texture_;
    sf::Sprite render_sprite_;
};
}  // namespace interface

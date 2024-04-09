#include "MainScreen.h"
#include <cmath>
namespace interface {
MainScreen::RGBA MainScreen::RGBA::FromRGB(renderer::RGB color) {
    RGBA ans;
    ans.R = color.GetRi();
    ans.G = color.GetGi();
    ans.B = color.GetBi();
    return ans;
}
MainScreen::MainScreen(int width, int height)
    : recieve_port_(
          RecievePortType::doNothing, [this](const renderer::Screen& screen) { Draw(screen); },
          RecievePortType::doNothing),
      window_(sf::VideoMode(width, height), "renderer") {
}

void MainScreen::Draw(const renderer::Screen& screen) {
    render_texture_.loadFromImage(MakeImage(screen));
    render_sprite_.setTexture(render_texture_);
    window_.draw(render_sprite_);
    window_.display();
}
sf::Image MainScreen::MakeImage(const renderer::Screen& screen) {
    sf::Image image;
    image.create(screen.GetWidth(), screen.GetHeight());
    for (int i = 0; i < screen.GetWidth(); ++i) {
        for (int j = 0; j < screen.GetHeight(); ++j) {
            RGBA color = RGBA::FromRGB(screen.GetPixel(j, i));
            image.setPixel(i, j, sf::Color(color.R, color.G, color.B, color.A));
        }
    }
    return image;
}
bool MainScreen::GetEvent(sf::Event& event) {
    return window_.waitEvent(event);
}
MainScreen::RecievePortType* MainScreen::GetRecievePort() {
    return &recieve_port_;
}
}  // namespace interface

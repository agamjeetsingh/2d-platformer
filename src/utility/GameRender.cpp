//
// Created by Agamjeet Singh on 27/08/25.
//

#include "../../include/utility/GameRender.h"

void GameRender::render(sf::RenderWindow &window, float dt) {
    if (shaking) {
        shake_time += dt;
        float offsetAmount = shake_amplitude * std::sin(2 * M_PI * shake_frequency * shake_time) * std::exp(-shake_damping * shake_time);
        sf::Vector2f offset = shake_direction * offsetAmount;
        sf::View shakenView = render_texture.getDefaultView();
        shakenView.move(offset);
        render_texture.setView(shakenView);
        if (std::abs(offsetAmount) < 0.1) {
            shaking = false;
            shake_time = 0.f;
        }
    }
    render_texture.clear(sf::Color::White);
    auto it = drawables.begin();
    while (it != drawables.end()) {
        if (auto sprite = it->first(dt)) {
            render_texture.draw(**sprite);
            ++it;
        } else {
            it = drawables.erase(it);
        }
    }

    render_texture.display();

    sf::Sprite sprite(render_texture.getTexture());
    sprite.setScale({4, 4});
    window.draw(sprite);

}

GameRender::GameRender() {
    if (!render_texture.resize({sf::VideoMode::getDesktopMode().size})) {
        const auto size = sf::VideoMode::getDesktopMode().size;
        std::cerr << "Error: Could not resize render_texture to desktop size x=" << size.x << " y=" << size.y << std::endl;
    }
    render_texture.clear(sf::Color::White);
}

GameRender &GameRender::getInstance() {
    static GameRender instance;
    return instance;
}



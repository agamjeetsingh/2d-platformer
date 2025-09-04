//
// Created by Agamjeet Singh on 27/08/25.
//

#include "../../include/utility/GameRender.h"

void GameRender::render(sf::RenderWindow &window) {
    auto it = drawables.begin();
    while (it != drawables.end()) {
        if (auto sprite = (*it)()) {
            render_texture.draw(*sprite);
            ++it;
        } else {
            it = drawables.erase(it);
        }
    }

    render_texture.display();

    sf::Sprite sprite(render_texture.getTexture());
    sprite.setScale({4, 4});
    window.draw(sprite);
    render_texture.clear(sf::Color::White);
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

void GameRender::drawSimpleDrawable(const sf::Drawable &sprite) {
    // No texture should be needed to be alive
    render_texture.draw(sprite);
}




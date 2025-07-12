#include <SFML/Graphics.hpp>
#include "include/utility/InputManager.h"

using namespace sf::Keyboard;

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My Game");
    sf::Clock clock;
    constexpr sf::Vector2f velocity = {100, 100};

    sf::Vector2f docsoc_coord = {100, 100};

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        const float dt = deltaTime.asSeconds();

        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<sf::Event::KeyPressed>()) {
                InputManager::getInstance().keyPressed(event->getIf<sf::Event::KeyPressed>()->code);
            }

            if (event->is<sf::Event::KeyReleased>()) {
                InputManager::getInstance().keyReleased(event->getIf<sf::Event::KeyReleased>()->code);
            }
        }

        if (InputManager::getInstance().isPressed(Key::W)) docsoc_coord.y -= velocity.y * dt;
        if (InputManager::getInstance().isPressed(Key::A)) docsoc_coord.x -= velocity.x * dt;
        if (InputManager::getInstance().isPressed(Key::S)) docsoc_coord.y += velocity.y * dt;
        if (InputManager::getInstance().isPressed(Key::D)) docsoc_coord.x += velocity.x * dt;

        window.clear(sf::Color::White);

        sf::Texture texture;
        if (!texture.loadFromFile("../docsoc.jpeg")) {
            break;
        }

        sf::Sprite sprite(texture);

        sprite.setPosition(docsoc_coord);

        window.draw(sprite);

        window.display();
    }
    return 0;
}

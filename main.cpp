#include <SFML/Graphics.hpp>

#include "include/entity/player/Player.h"
#include "include/entity/player/PlayerInputHandler.h"
#include "include/physics/CollisionsHandler.h"
#include "include/physics/ContactsHandler.h"
#include "include/utility/InputManager.h"

using namespace sf::Keyboard;

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.size), "My Game");
    sf::Clock clock;
    sf::Texture texture;
    if (!texture.loadFromFile("../docsoc.jpeg")) {
        return -1;
    }
    sf::Texture player_texture;
    if (!player_texture.loadFromFile("../player.png")) {
        return -1;
    }
    sf::Texture idle_texture;
    if (!idle_texture.loadFromFile("../assets/player/idle00.png")) {
        return -1;
    }
    sf::Sprite real_player_sprite(idle_texture);
    real_player_sprite.setScale({4, 4});
    std::vector<sf::FloatRect> hitbox = {sf::FloatRect({0, 0}, sf::Vector2<float>(texture.getSize()))};
    sf::Sprite sprite(texture);
    sf::Sprite player_sprite(player_texture);
    player_sprite.setScale({4, 4});
    sprite.setScale({4, 4});
    Player player({sf::FloatRect({0, 0}, sf::Vector2<float>(idle_texture.getSize()))}, real_player_sprite, {100, 100});
    Player player2({sf::FloatRect({0, 0}, sf::Vector2<float>(player_texture.getSize()))}, player_sprite, {400, 400});
    sprite.setScale({1, 1});
    CollidableObject box(hitbox, sprite, {0, 300}, CollidableObjectType::Immovable);
    // box.intrinsic_velocity.x = 100;
    CollidableObject box_2(hitbox, sprite, {800, 100}, CollidableObjectType::Immovable);
    // CollidableObject box_3(hitbox, sprite, {0, -200}, CollidableObjectType::Immovable);
    // box_3.acceleration = {0, 800};
    player.acceleration = {0, 800};

    PlayerInputHandler input_handler =  PlayerInputHandler{player};

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        const float dt = std::min(deltaTime.asSeconds(), 0.1f);

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

        // input_handler.update();

        window.clear(sf::Color::White);

        CollisionsHandler::getInstance().update(dt, player);

        player.updateSprite(dt);

        std::cout << "About to draw player sprite at position: " << player.getSprite().getPosition().x << ", "
          << player.getSprite().getPosition().y << std::endl;

        sf::FloatRect bounds = player.getSprite().getLocalBounds();
        std::cout << "Sprite bounds: " << bounds.size.x << "," << bounds.size.y << "x" << bounds.size.x << ", " << bounds.size.y << std::endl;

        std::cout << "Is player on land???? The verdict: " << (ContactsHandler::getInstance().onLand(player) ? "yes" : "no") << std::endl;

        window.draw(player.getSprite());
        window.draw(player2.getSprite());
        window.draw(box.getSprite());
        CollisionsHandler::getInstance().drawHitboxes(window);

        std::cout << "Player 1 x: " << player.getSprite().getPosition().x << "y: " << player.getSprite().getPosition().y << std::endl;
        std::cout << "Player 2 x: " << player2.getSprite().getPosition().x << "y: " << player2.getSprite().getPosition().y << std::endl;
        std::cout << "Player 1 Hitbox x: " << player.getHitbox().getRects()[0].position.x << "y: " << player.getHitbox().getRects()[0].position.y << std::endl;
        std::cout << "Player 2 Hitbox x: " << player2.getHitbox().getRects()[0].position.x << "y: " << player2.getHitbox().getRects()[0].position.y << std::endl;
        player.printVelocity("Player 1");
        player2.printVelocity("Player 2");
        std::cout << "Player 1 Acceleration x: " << player.acceleration.x << "y: " << player.acceleration.y << std::endl;
        std::cout << "Player 2 Acceleration x: " << player2.acceleration.x << "y: " << player2.acceleration.y << std::endl;
        std::cout << CollisionsHandler::getInstance().getBodies().size() << std::endl;
        std::cout << "--------------------" << std::endl;

        auto contacts = ContactsHandler::getInstance().getContacts();

        auto player_contacts = ContactsHandler::getInstance().allContacts(player);

        sf::Font font;
        if (!font.openFromFile("/System/Library/Fonts/Supplemental/arial.ttf")) { // Change path for Linux/Mac
            std::cerr << "Could not load font!\n";
            return -1;
        }

        sf::Text text = font;
        text.setString("Intrinsic velocity: x = " + std::to_string(player.intrinsic_velocity.x) +
            ", y = " + std::to_string(player.intrinsic_velocity.y));
        text.setCharacterSize(25); // in pixels
        text.setFillColor(sf::Color::Black);
        text.setPosition({0.f, 0.f});

        sf::Text text2 = font;
        text2.setString("Friction velocity: x = " + std::to_string(player.friction_velocity.x) + ", y = " +
            std::to_string(player.friction_velocity.y));
        text2.setCharacterSize(25);
        text2.setFillColor(sf::Color::Black);
        text2.setPosition({0, 30});

        sf::Text text3 = font;
        text3.setString("Impulse velocity: x = " + std::to_string(player.impulse_velocity.x) + ", y = " +
            std::to_string(player.impulse_velocity.y));
        text3.setCharacterSize(25);
        text3.setFillColor(sf::Color::Black);
        text3.setPosition({0, 60});

        sf::Text text4 = font;
        text4.setString("onGround: " + std::to_string(player.isOnGround()));
        text4.setCharacterSize(25);
        text4.setFillColor(sf::Color::Black);
        text4.setPosition({0, 90});

        window.draw(text);
        window.draw(text2);
        window.draw(text3);
        window.draw(text4);

        window.display();
    }
    return 0;
}

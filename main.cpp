#include <SFML/Graphics.hpp>

#include "include/entity/player/Player.h"
#include "include/entity/player/PlayerInputHandler.h"
#include "include/physics/CollisionsHandler.h"
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
    CollidableObject box(hitbox, sprite, {0, 200}, CollidableObjectType::Immovable);
    box.velocity.x = 50;

    player.acceleration = {0, 800};

    PlayerInputHandler input_handler =  PlayerInputHandler{player};

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

        // if (InputManager::getInstance().isPressed(Key::W)) player.velocity.y = -200;
        // if (InputManager::getInstance().isPressed(Key::A)) player.velocity.x = -200;
        // if (InputManager::getInstance().isPressed(Key::S)) player.velocity.y = 200;
        // if (InputManager::getInstance().isPressed(Key::D)) player.velocity.x = 200;

        // if (InputManager::getInstance().isPressed(Key::W)) player.velocity.y = -200;
        // if (InputManager::getInstance().isPressed(Key::A)) {
        //     player.velocity.x = -200;
        //     player.facing = PlayerFacing::Left;
        // } else {
        //     player.velocity.x = 0;
        // }
        // if (InputManager::getInstance().isPressed(Key::S)) player.velocity.y = 200;
        // if (InputManager::getInstance().isPressed(Key::D)) {
        //     player.velocity.x = 200;
        //     player.facing = PlayerFacing::Right;
        // } else if (!InputManager::getInstance().isPressed(Key::A)) {
        //     player.velocity.x = 0;
        // }

        input_handler.update();

        window.clear(sf::Color::White);

        CollisionsHandler::getInstance().update(dt);

        player.updates(dt);

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
        std::cout << "Player 1 Velocity x: " << player.velocity.x << "y: " << player.velocity.y << std::endl;
        std::cout << "Player 2 Velocity x: " << player2.velocity.x << "y: " << player2.velocity.y << std::endl;
        std::cout << "Player 1 Acceleration x: " << player.acceleration.x << "y: " << player.acceleration.y << std::endl;
        std::cout << "Player 2 Acceleration x: " << player2.acceleration.x << "y: " << player2.acceleration.y << std::endl;
        std::cout << CollisionsHandler::getInstance().getBodies().size() << std::endl;
        std::cout << "--------------------" << std::endl;

        std::cout << "Island Info" << std::endl;
        std::cout << "Player island size: " << player.getIsland()->getMembers().size() << std::endl;
        std::cout << "Player in global island or not: " << (player.getIsland() == GlobalIsland::getInstance()) << std::endl;
        std::cout << "Player island velocity" << (player.getIslandVelocity().x) << std::endl;
        std::cout << "Box island size: " << box.getIsland()->getMembers().size() << std::endl;
        std::cout << "Box in global island or not: " << (box.getIsland() == GlobalIsland::getInstance()) << std::endl;
        assert(&GlobalIsland::getInstance()->getLeader() != &box);
        std::cout << "--------------------" << std::endl;
        std::cout << "--------------------" << std::endl;

        auto contacts = ContactsHandler::getInstance().getContacts();
        if (!contacts.empty()) {

        }

        auto player_contacts = ContactsHandler::getInstance().allContacts(player);
        if (!player_contacts.empty()) {

        }

        window.display();
    }
    return 0;
}

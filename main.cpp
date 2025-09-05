#include <SFML/Graphics.hpp>

#include "events/EventBus.h"
#include "include/entity/player/Player.h"
#include "include/entity/player/PlayerInputHandler.h"
#include "include/physics/CollisionsHandler.h"
#include "include/physics/ContactsHandler.h"
#include "include/utility/InputManager.h"
#include "utility/GameRender.h"
#include "utility/SoundManager.h"

using namespace sf::Keyboard;

int main() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(sf::VideoMode(desktop.size), "My Game");
    sf::Clock clock;
    SoundManager::loadBuffers();
    sf::Texture texture;
    if (!texture.loadFromFile("../docsoc.jpeg")) {
        return -1;
    }
    sf::Texture player_texture;
    if (!player_texture.loadFromFile("../player.png")) {
        return -1;
    }
    sf::Texture idle_texture;
    if (!idle_texture.loadFromFile("../assets/player/idle/idle00.png")) {
        return -1;
    }

    ///
    sf::Texture dash_texture;
    if (!dash_texture.loadFromFile("../assets/player/dash_transparent00.png")) {
        std::cerr << "Error: Couldn't load from file: " << std::endl;
    }
    sf::Sprite dash_sprite(dash_texture);
    dash_sprite.setPosition({500, 300});

    ///

    sf::Sprite real_player_sprite(idle_texture);
    // real_player_sprite.setColor({0, 255, 255, 100});
    // real_player_sprite.setScale({4, 4});
    std::vector<sf::FloatRect> hitbox = {sf::FloatRect({0, 0}, sf::Vector2<float>(texture.getSize()))};
    sf::Sprite sprite(texture);
    sf::Sprite player_sprite(player_texture);
    player_sprite.setScale({4, 4});
    sprite.setScale({4, 4});


    auto player = std::make_shared<Player>(
        std::vector{ sf::FloatRect({0, 0}, {13, 12}) },
        real_player_sprite,
        sf::Vector2f{50, 0}
    );

    GameRender::getInstance().registerDrawable(player);

    // Player player2({sf::FloatRect({0, 0}, sf::Vector2<float>(player_texture.getSize()))}, player_sprite, {400, 400});
    sprite.setScale({1, 1});
    auto box = std::make_shared<CollidableObject>(hitbox, sprite, sf::Vector2f{0, 200}, CollidableObjectType::Immovable);
    // box->base_velocity = {20, 0};

    GameRender::getInstance().registerDrawable(box);

    CollidableObject box_2(hitbox, sprite, {800, 100}, CollidableObjectType::Immovable);
    // box_2.intrinsic_velocity.x = -100;
    // CollidableObject box_3(hitbox, sprite, {0, -200}, CollidableObjectType::Immovable);
    // box_3.acceleration = {0, 800};

    PlayerInputHandler input_handler =  PlayerInputHandler{*player};

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

        window.clear(sf::Color::White);

        EventBus::getInstance().execute(EventExecuteTime::PRE_INPUT);

        PlayerInputHandler{*player}.update(dt);

        EventBus::getInstance().execute(EventExecuteTime::PRE_PHYSICS);

        CollisionsHandler::getInstance().update(dt);

        EventBus::getInstance().execute(EventExecuteTime::POST_PHYSICS);

        player->updateSprite(dt);

        Scheduler::getInstance().update(dt);

        GameRender::getInstance().render(window);

        // window.draw(dash_sprite);

        // Rough drawing code

        // window.draw(player->getSprite());
        // window.draw(player2.getSprite());
        // window.draw(box.getSprite());

        CollisionsHandler::getInstance().drawHitboxes();
        // Debug Statements Start

        std::cout << "About to draw player sprite at position: " << player->getSprite()->getPosition().x << ", "
          << player->getSprite()->getPosition().y << std::endl;

        sf::FloatRect bounds = player->getSprite()->getLocalBounds();
        std::cout << "Sprite bounds: " << bounds.size.x << "," << bounds.size.y << "x" << bounds.size.x << ", " << bounds.size.y << std::endl;

        std::cout << "Is player on land???? The verdict: " << (ContactsHandler::getInstance().onLand(*player) ? "yes" : "no") << std::endl;





        std::cout << "Player 1 x: " << player->getSprite()->getPosition().x << "y: " << player->getSprite()->getPosition().y << std::endl;
        // std::cout << "Player 2 x: " << player2.getSprite().getPosition().x << "y: " << player2.getSprite().getPosition().y << std::endl;
        std::cout << "Player 1 Hitbox x: " << player->getHitbox().getRects()[0].position.x << "y: " << player->getHitbox().getRects()[0].position.y << std::endl;
        // std::cout << "Player 2 Hitbox x: " << player2.getHitbox().getRects()[0].position.x << "y: " << player2.getHitbox().getRects()[0].position.y << std::endl;
        player->printVelocity("Player 1");
        // player2.printVelocity("Player 2");
        std::cout << "Player 1 Acceleration x: " << player->gravity_acceleration.x << "y: " << player->gravity_acceleration.y << std::endl;
        // std::cout << "Player 2 Acceleration x: " << player2.gravity_acceleration.x << "y: " << player2.gravity_acceleration.y << std::endl;
        std::cout << CollisionsHandler::getInstance().getBodies().size() << std::endl;
        std::cout << "--------------------" << std::endl;

        sf::Font font;
        if (!font.openFromFile("/System/Library/Fonts/Supplemental/arial.ttf")) { // Change path for Linux/Mac
            std::cerr << "Could not load font!\n";
            return -1;
        }

        sf::Text text = font;
        text.setString("Intrinsic velocity: x = " + std::to_string(player->base_velocity.x) +
            ", y = " + std::to_string(player->base_velocity.y));
        text.setCharacterSize(25); // in pixels
        text.setFillColor(sf::Color::Black);
        text.setPosition({0.f, 0.f});

        sf::Text text2 = font;
        text2.setString("Friction velocity: x = " + std::to_string(player->friction_velocity.x) + ", y = " +
            std::to_string(player->friction_velocity.y));
        text2.setCharacterSize(25);
        text2.setFillColor(sf::Color::Black);
        text2.setPosition({0, 30});

        // sf::Text text3 = font;
        // text3.setString("Impulse velocity: x = " + std::to_string(player->impulse_velocity.x) + ", y = " +
        //     std::to_string(player->impulse_velocity.y));
        // text3.setCharacterSize(25);
        // text3.setFillColor(sf::Color::Black);
        // text3.setPosition({0, 60});

        // sf::Text text4 = font;
        // text4.setString("Gravity velocity: y = " + std::to_string(player->getGravityVelocity().y));
        // text4.setCharacterSize(25);
        // text4.setFillColor(sf::Color::Black);
        // text4.setPosition({0, 90});

        sf::Text text5 = font;
        text5.setString("onGround: " + std::to_string(ContactsHandler::getInstance().onLand(*player)) + std::to_string(player->isOnGround()) + " (according to onLand, player->onGround)");
        text5.setCharacterSize(25);
        text5.setFillColor(sf::Color::Black);
        text5.setPosition({0, 120});

        std::cout << ContactsHandler::getInstance().onLand(*player) << std::endl;
        // assert(ContactsHandler::getInstance().onLand(player) == player->isOnGround());


        sf::Text text6 = font;
        text6.setString(std::string("Can jump: ") + std::to_string(player->canJump()) + " variable jump: " + std::to_string(player->ability_jump.isPerforming()));
        text6.setCharacterSize(25);
        text6.setFillColor(sf::Color::Black);
        text6.setPosition({0, 150});

        window.draw(text);
        window.draw(text2);
        // window.draw(text3);
        // window.draw(text4);
        window.draw(text5);
        window.draw(text6);

        // Debug statements end

        window.display();
    }
    return 0;
}

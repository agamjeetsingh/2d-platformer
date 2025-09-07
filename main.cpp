#include <SFML/Graphics.hpp>

#include "entity/objects/OneWayPlatform.h"
#include "entity/objects/TouchSwitch.h"
#include "events/EventBus.h"
#include "include/entity/player/Player.h"
#include "include/entity/player/PlayerInputHandler.h"
#include "include/physics/CollisionsHandler.h"
#include "include/physics/ContactsHandler.h"
#include "include/utility/InputManager.h"
#include "utility/EmptyTextures.h"
#include "utility/GameRender.h"
#include "utility/SoundManager.h"

using namespace sf::Keyboard;

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().size), "My Game");

    sf::Clock clock;
    SoundManager::loadBuffers();
    sf::Texture texture;
    if (!texture.loadFromFile("../docsoc.jpeg")) {
        return -1;
    }
    std::vector<sf::FloatRect> hitbox = {sf::FloatRect({0, 0}, sf::Vector2<float>(texture.getSize()))};
    sf::Sprite sprite(texture);

    auto player = std::make_shared<Player>(
        std::vector{ sf::FloatRect({0, 0}, {13, 12}) },
        sf::Vector2f{50, 0}
    );

    GameRender::getInstance().registerDrawable(player);

    sprite.setScale({1, 1});
    auto box = std::make_shared<CollidableObject>(hitbox, sprite, sf::Vector2f{0, 200}, CollidableObjectType::Immovable);
    // box->base_velocity = {20, 0};

    GameRender::getInstance().registerDrawable(box);

    auto touch_switches = TouchSwitch::makeTouchSwitches({{100, 75}, {180, 180}});

    for (const auto& ptr: touch_switches) {
        GameRender::getInstance().registerDrawable(ptr, 10);
    }

    auto one_way_platform = std::make_shared<OneWayPlatform>(sf::Vector2f{100, 150});

    GameRender::getInstance().registerDrawable(one_way_platform);

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

        Scheduler::getInstance().update(dt);

        GameRender::getInstance().render(window, dt);

        CollisionsHandler::getInstance().drawHitboxes();
        // Debug Statements Start

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

        sf::Text text5 = font;
        text5.setString("onGround: " + std::to_string(ContactsHandler::getInstance().onLand(*player)) + std::to_string(player->isOnGround()) + " (according to onLand, player->onGround)");
        text5.setCharacterSize(25);
        text5.setFillColor(sf::Color::Black);
        text5.setPosition({0, 120});

        sf::Text text6 = font;
        text6.setString(std::string("Can jump: ") + std::to_string(player->canJump()) + " variable jump: " + std::to_string(player->ability_jump.isPerforming()));
        text6.setCharacterSize(25);
        text6.setFillColor(sf::Color::Black);
        text6.setPosition({0, 150});

        window.draw(text);
        window.draw(text2);
        window.draw(text5);
        window.draw(text6);

        // Debug statements end

        window.display();
    }

    EmptyTextures::getInstance().cleanup();
    return 0;
}

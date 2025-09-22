#include <SFML/Graphics.hpp>

#include "entity/PointParticles.h"
#include "entity/SpriteParticles.h"
#include "entity/objects/DashCrystal.h"
#include "entity/objects/KillZone.h"
#include "entity/objects/OneWayPlatform.h"
#include "entity/objects/SwapBlock.h"
#include "entity/objects/TouchSwitch.h"
#include "events/EventBus.h"
#include "include/entity/player/Player.h"
#include "include/entity/player/PlayerInputHandler.h"
#include "include/physics/2d/CollisionsHandler.h"
#include "include/utility/InputManager.h"
#include "utility/EmptyTextures.h"
#include "utility/GameLevel.h"
#include "utility/GameRender.h"
#include "utility/SoundManager.h"

using namespace sf::Keyboard;

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().size), "My Game");
    std::cout << sf::VideoMode::getDesktopMode().size.x << " " << sf::VideoMode::getDesktopMode().size.y << std::endl;
    sf::Clock clock;
    SoundManager::loadBuffers();

    CollisionsHandler collisions_handler;

    auto player = std::make_shared<Player>(
        std::vector{ sf::FloatRect({0, 0}, {13, 12}) },
        std::vector{ sf::FloatRect({0, 4}, {13, 8}) },
        sf::Vector2f{50, 0}
    );
    collisions_handler.addObject(*player);

    GameRender::getInstance().registerDrawable(player);

    // auto box = std::make_shared<CollidableObject>(hitbox, sprite, sf::Vector2f{0, 200}, CollidableObjectType::Immovable);
    // // box->base_velocity = {20, 0};
    //
    // GameRender::getInstance().registerDrawable(box);

    auto touch_switches = TouchSwitch::makeTouchSwitches({{100, 75}, {180, 180}});
    for (const auto& touch_switch: touch_switches) {
        collisions_handler.addObject(*touch_switch);
    }

    for (const auto& ptr: touch_switches) {
        GameRender::getInstance().registerDrawable(ptr, 10);
    }

    // auto one_way_platform = std::make_shared<OneWayPlatform>(sf::Vector2f{100, 150});
    //
    // GameRender::getInstance().registerDrawable(one_way_platform);
    //
    auto kill_zone = std::make_shared<KillZone>(sf::Vector2f{250, 150}, sf::Vector2f{10, 10});
    GameRender::getInstance().registerDrawable(kill_zone);
    collisions_handler.addObject(*kill_zone);

    // auto dash_crystal = std::make_shared<DashCrystal>(sf::Vector2f{50, 150});
    // GameRender::getInstance().registerDrawable(dash_crystal);
    //
    // auto swap_block = std::make_shared<SwapBlock>(sf::Vector2f{20, 170}, sf::Vector2f{350, 170});
    // GameRender::getInstance().registerDrawable(swap_block);

    // auto particles = std::make_shared<PointParticles>(1, PointParticles::FadingColor(sf::Color::Black), PointParticles::ExpDampedVelocity({10, 10}, 1), std::vector{sf::Vector2f{10, 100}}, std::vector{15.f});
    // GameRender::getInstance().registerDrawable(particles);

    // auto more_particles = std::make_shared<PointParticles>(6, PointParticles::FadingColor(sf::Color::Blue), PointParticles::ExpDampedVelocity({10, 10}, 1), Random::Vector2fRange({20, 100}, {30, 110}), Random::FloatRange(0.5, 1.2));
    // GameRender::getInstance().registerDrawable(more_particles);

    auto snow_particles = std::make_shared<PointParticles>(600, PointParticles::ConstantColor(sf::Color::White), PointParticles::RandomSinWaveVelocity(Random::FloatRange(100, 180), Random::FloatRange(4, 10), Random::FloatRange(0.02, 0.03), Random::FloatRange(0, 1)), Random::Vector2fRange({-6.f * sf::VideoMode::getDesktopMode().size.x, 0}, {0,200}), Random::FloatRange(100, 100));
    GameRender::getInstance().registerDrawable(snow_particles);

    // auto sprite_particles = std::make_shared<SpriteParticles>(std::vector(2, TexturesHolder(4, "../assets/particles/smoke")), std::vector<sf::Vector2f>{{150, 150}, {160, 160}}, std::vector<float>{10, 10}, SpriteParticles::ConstantVelocity({0, 0}));
    // GameRender::getInstance().registerDrawable(sprite_particles);
    //
    // auto sprite_particles_manager = SpriteParticlesManager();
    // sprite_particles_manager.createParticles(std::vector(2, TexturesHolder(4, "../assets/particles/smoke")), std::vector<sf::Vector2f>{{130, 130}, {120, 120}}, std::vector<float>{10, 10}, SpriteParticles::ConstantVelocity({0, 0}));

    // std::vector<std::shared_ptr<CollidableObject>> shared;
    // shared.reserve(1000);
    // for (int i = 0; i < 1000; i++) {
    //     shared.push_back(std::make_shared<CollidableObject>(std::vector<sf::FloatRect>{{sf::Vector2f{0, 0}, Random::Vector2fRange({1, 1}, {2, 2}).getRandom()}}, sprite));
    //     shared.back()->base_velocity.y = Random::FloatRange(1, 5).getRandom();
    //     shared.back()->setPosition(Random::Vector2fRange({0, 0}, {200, 200}).getRandom());
    //     shared.back()->gravity_acceleration = {0, 80};
    // }

    GameLevel game_level(collisions_handler);
    game_level.load();

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = std::min(deltaTime.asSeconds(), 0.033f);
        // dt /= 8; // SLOW-MO

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

        collisions_handler.update(dt);

        EventBus::getInstance().execute(EventExecuteTime::POST_PHYSICS);

        Scheduler::getInstance().update(dt);

        GameRender::getInstance().render(window, dt);

        collisions_handler.drawHitboxes();
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

        sf::Text text3 = font;
        text3.setString("FPS: " + std::to_string(10 * (static_cast<int>(1 / dt) / 10)));
        text3.setCharacterSize(25);
        text3.setFillColor(sf::Color::Black);
        text3.setPosition({0, 60});

        sf::Text text4 = font;
        text4.setString("x: " + std::to_string(player->getPosition().x) + " y: " + std::to_string(player->getPosition().y));
        text4.setCharacterSize(25);
        text4.setFillColor(sf::Color::Black);
        text4.setPosition({0, 90});

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
        window.draw(text3);
        window.draw(text4);
        window.draw(text5);
        window.draw(text6);

        // Debug statements end

        window.display();
    }

    EmptyTextures::getInstance().cleanup();
    return 0;
}

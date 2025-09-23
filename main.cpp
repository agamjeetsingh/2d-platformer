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

    SoundManager<SoundEffect> sound_manager;

    sound_manager.registerSoundEffect(SoundEffect::DASH_LEFT, "../assets/player/audio/dash_red_left.wav");
    sound_manager.registerSoundEffect(SoundEffect::DASH_RIGHT, "../assets/player/audio/dash_red_right.wav");
    sound_manager.registerSoundEffect(SoundEffect::LAND, "../assets/player/audio/land_00_dirt_01.wav");
    sound_manager.registerSoundEffect(SoundEffect::TOUCH_SWITCH_ACTIVATE, "../assets/touchSwitch/touchswitch_any.wav");
    sound_manager.registerSoundEffect(SoundEffect::TOUCH_SWITCH_ALL_ACTIVATE, "../assets/touchSwitch/touchswitch_last.wav");
    sound_manager.registerSoundEffect(SoundEffect::DEATH, "../assets/player/death.wav");
    sound_manager.registerSoundEffect(SoundEffect::DIAMOND_RETURN_01, "../assets/dashRefill/diamond_return_01.wav");
    sound_manager.registerSoundEffect(SoundEffect::DIAMOND_RETURN_02, "../assets/dashRefill/diamond_return_02.wav");
    sound_manager.registerSoundEffect(SoundEffect::DIAMOND_RETURN_03, "../assets/dashRefill/diamond_return_03.wav");
    sound_manager.registerSoundEffect(SoundEffect::DIAMOND_TOUCH_01, "../assets/dashRefill/diamond_touch_01.wav");
    sound_manager.registerSoundEffect(SoundEffect::DIAMOND_TOUCH_02, "../assets/dashRefill/diamond_touch_02.wav");
    sound_manager.registerSoundEffect(SoundEffect::DIAMOND_TOUCH_03, "../assets/dashRefill/diamond_touch_03.wav");
    sound_manager.registerSoundEffect(SoundEffect::SWAP_BLOCK_MOVE, "../assets/swapBlock/swapblock_move.wav");
    sound_manager.registerSoundEffect(SoundEffect::SWAP_BLOCK_RETURN_LOOP_INTRO, "../assets/swapBlock/swapblock_return_loop_intro.wav");
    sound_manager.registerSoundEffect(SoundEffect::SWAP_BLOCK_RETURN_LOOP, "../assets/swapBlock/swapblock_return_loop.wav");
    sound_manager.registerSoundEffect(SoundEffect::SWAP_BLOCK_RETURN_LOOP_END, "../assets/swapBlock/swapblock_return_loop_end.wav");

    CollisionsHandler collisions_handler;
    auto player = std::make_shared<Player>(
        std::vector{ sf::FloatRect({0, 0}, {13, 12}) },
        std::vector{ sf::FloatRect({0, 4}, {13, 8}) },
        sound_manager,
        sf::Vector2f{50, 0}
    );
    collisions_handler.addObject(*player);

    GameRender::getInstance().registerDrawable(player);

    auto touch_switches = TouchSwitch::makeTouchSwitches({{100, 75}, {180, 180}}, sound_manager);
    for (const auto& touch_switch: touch_switches) {
        collisions_handler.addObject(*touch_switch);
    }

    for (const auto& ptr: touch_switches) {
        GameRender::getInstance().registerDrawable(ptr, 10);
    }

    auto snow_particles = std::make_shared<PointParticles>(600, PointParticles::ConstantColor(sf::Color::White), PointParticles::RandomSinWaveVelocity(Random::FloatRange(100, 180), Random::FloatRange(4, 10), Random::FloatRange(0.02, 0.03), Random::FloatRange(0, 1)), Random::Vector2fRange({-6.f * sf::VideoMode::getDesktopMode().size.x, 0}, {0,200}), Random::FloatRange(100, 100));
    GameRender::getInstance().registerDrawable(snow_particles);

    GameLevel game_level(collisions_handler, sound_manager);
    game_level.load("../assets/levels/map.json");

    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = std::min(deltaTime.asSeconds(), 0.033f);

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

        window.display();
    }

    EmptyTextures::getInstance().cleanup();
    return 0;
}

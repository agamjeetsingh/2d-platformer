//
// Created by Agamjeet Singh on 08/09/25.
//

#include "../../../include/entity/objects/SwapBlock.h"

#include "events/EventBus.h"

SwapBlock::SwapBlock(sf::Vector2f starting_pos, sf::Vector2f ending_pos, SoundManager<SoundEffect>& sound_manager, EventBus& now_bus): CollidableObject({{{0, 0}, {24, 24}}}, sf::Sprite{EmptyTextures::getInstance().getEmpty({24, 24})}, starting_pos, CollidableObjectType::Immovable),
                                                                          dash_listener(Listener::make_listener<PlayerDashEvent>(now_bus, [this](const PlayerDashEvent& event) {
                                                                              // Starting swap
                                                                              base_velocity = swap_velocity;
                                                                              at_start = false;
                                                                              at_end = false;
                                                                              going = true;
                                                                              if (wait_at_end) wait_at_end->cancel();
                                                                              if (moving_back) moving_back->cancel();
                                                                              if (reached_beginning) reached_beginning->cancel();
                                                                              sound_loop.abort();

                                                                              wait_at_end = Scheduler::getInstance().schedule([this](std::shared_ptr<ScheduledEvent> event, float dt) {
                                                                                  // Reached end
                                                                                  base_velocity = {0, 0};
                                                                                  at_end = true;
                                                                                  at_start = false;
                                                                                  moving_back = Scheduler::getInstance().schedule([this](std::shared_ptr<ScheduledEvent> event, float dt) {
                                                                                      // Rest over, time to come back
                                                                                      base_velocity = swap_return_velocity;
                                                                                      at_end = false;
                                                                                      at_start = false;
                                                                                      going = false;
                                                                                      sound_loop.start();
                                                                                      reached_beginning = Scheduler::getInstance().schedule([this](std::shared_ptr<ScheduledEvent> event, float dt) {
                                                                                          // Reached beginning
                                                                                          sound_loop.end();
                                                                                          base_velocity = {0, 0};
                                                                                          at_end = false;
                                                                                          at_start = true;
                                                                                      }, swap_return_time);
                                                                                  }, TIME_AT_END);
                                                                              }, swap_time);
                                                                          })), starting_pos(starting_pos), ending_pos(ending_pos), sound_manager(sound_manager) {
    assert(start_texture.loadFromFile("../assets/swapBlock/blockRed.png"));
    assert(end_texture.loadFromFile("../assets/swapBlock/block.png"));
}

void SwapBlock::updateSprite(float deltaTime) {
    if (at_start || !going) {
        sprite.setTexture(start_texture);
    } else {
        sprite.setTexture(end_texture);
    }
}

void SwapBlock::addPosition(sf::Vector2f position) {
    if ((getPosition() + position - starting_pos).length() >= (ending_pos - starting_pos).length()) {
        setPosition(ending_pos);
    } else {
        CollidableObject::addPosition(position);
    }
}

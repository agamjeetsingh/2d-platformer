//
// Created by Agamjeet Singh on 08/09/25.
//

#ifndef SWAPBLOCK_H
#define SWAPBLOCK_H
#include "entity/CollidableObject.h"
#include "events/Listener.h"
#include "events/PlayerDashEvent.h"
#include "utility/EmptyTextures.h"
#include "utility/SoundLoop.h"


class SwapBlock final : public CollidableObject {
public:
    SwapBlock(sf::Vector2f starting_pos, sf::Vector2f ending_pos);

    void updateSprite(float deltaTime);

    void addPosition(sf::Vector2f position) override;

    static constexpr float TIME_AT_END = 0.8; // In seconds
    static constexpr float SWAP_SPEED = 720;
    static constexpr float SWAP_RETURN_SPEED = 80;

private:
    Listener dash_listener;
    sf::Vector2f starting_pos;
    sf::Vector2f ending_pos;
    sf::Vector2f swap_velocity = (ending_pos - starting_pos).normalized() * SWAP_SPEED;
    sf::Vector2f swap_return_velocity = (starting_pos - ending_pos).normalized() * SWAP_RETURN_SPEED;
    float swap_time = (ending_pos - starting_pos).length() / SWAP_SPEED; // In seconds
    float swap_return_time = (starting_pos - ending_pos).length() / SWAP_RETURN_SPEED; // In seconds
    bool at_start = true;
    bool at_end = false;
    bool going = false; // going -> green otherwise red

    std::shared_ptr<ScheduledEvent> wait_at_end;
    std::shared_ptr<ScheduledEvent> moving_back;
    std::shared_ptr<ScheduledEvent> reached_beginning;

    sf::Texture start_texture;
    sf::Texture end_texture;

    SoundLoop sound_loop{SoundEffect::SWAP_BLOCK_RETURN_LOOP_INTRO, SoundEffect::SWAP_BLOCK_RETURN_LOOP, SoundEffect::SWAP_BLOCK_RETURN_LOOP_END};
};



#endif //SWAPBLOCK_H

//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "Facing.h"
#include "PlayerState.h"
#include "../CollidableObject.h"
#include "sprites/PlayerSpriteHandler.h"
#include "../../utility/Scheduler.h"

class ContactsHandler;

class Player final : public CollidableObject {
public:
    Player(std::vector<sf::FloatRect> hitbox,
        sf::Sprite sprite,
        sf::Vector2f position = {0, 0});

    static constexpr float MAX_STAMINA = 110;
    static constexpr float JUMP_GRACE_TIME = 0.1; // In seconds
    static constexpr float WALK_SPEED = 64;

    PlayerState state = PlayerState::Idle;
    Facing facing = Facing::Right;
    float stamina = MAX_STAMINA;

    void flipDirection();

    void updates(float deltaTime);

    void restoreStamina();

    void restoreDash();

    /**
     * @brief Restores dash capacity and stamina. Must be called each frame the player is on ground.
     */
    void onLand();

    /**
     * Sets \code canJumpDueToGrace\endcode and schedules an event to set it to false after \code JUMP_GRACE_TIME\endcode
     * seconds. Must be called only at the frame the player leaves the ground. Should not be called every time the
     * player is in the air.
     */
    void noLongerOnGround();

    [[nodiscard]] bool canJump() const;

    /**
     * Sets \code canJumpDueToGrace\endcode flag to false. Must only be called at the frame the player jumps.
     */
    void justJumped();

    [[nodiscard]] bool canDash() const {
        return dashCapacity;
    }

    void updatePhysicsStates();

    [[nodiscard]] std::optional<std::reference_wrapper<CollidableObject>> isClimbing() const {
        return climbing;
    }

private:
    PlayerSpriteHandler sprite_handler = PlayerSpriteHandler(state, sprite, facing);

    // Physics tags updated by updatePhysicsStates()
    bool onGround = true;

    bool dashCapacity = true;
    bool canJumpDueToGrace = false;
    std::optional<std::reference_wrapper<CollidableObject>> climbing = std::nullopt;
};

#endif //PLAYER_H

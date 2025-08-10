//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "DashDirection.h"
#include "Facing.h"
#include "PlayerSpriteState.h"
#include "PlayerState.h"
#include "PlayerStateGround.h"
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
    static constexpr float JUMP_GRACE_COYOTE_TIME = 0.1; // In seconds
    static constexpr float JUMP_GRACE_LANDING_TIME = 0.2; // In seconds
    static constexpr float WALK_SPEED = 90;
    static constexpr float RUN_REDUCE = 400;
    static constexpr float RUN_ACCELERATION = 1000;
    static constexpr float AIR_MULTIPLIER = 0.65;
    static constexpr float JUMP_SPEED = -105;
    static constexpr float JUMP_H_BOOST = 40;
    static constexpr float UPWARD_CORNER_CORRECTION = 4;

    static constexpr float DASH_SPEED = 240;
    static constexpr float DASH_ATTACK_TIME = 0.1; // In seconds
    static constexpr float DASH_FREEZE_TIME = 0.05; // In seconds
    static constexpr float DASH_SPEED_TIME = 0.2; // In seconds
    static constexpr float TOTAL_DASH_TIME = DASH_FREEZE_TIME + DASH_SPEED_TIME;
    static constexpr float DASH_RESET_SPEED_HORIZONTAL = 160;
    static constexpr float DASH_RESET_SPEED_UP_DIAGONAL = 160;
    static constexpr float DASH_RESET_SPEED_VERTICAL = 120;
    static constexpr DashDirection DEFAULT_DASH_DIRECTION = DashDirection::RIGHT;
    // Dash reset speed for down diagonal is the current speed (it is retained)

    bool dash_reset_timer_over = true;
    float stamina = MAX_STAMINA;
    PlayerSpriteState sprite_state = PlayerSpriteState::Ground;
    Facing facing = Facing::Right;

    void updateSprite(float deltaTime);

    // ===== State Methods ====
    void changeStateTo(PlayerState& new_state);
    void runDuringState();
    PlayerState& getState() const;
    void updateState();

    bool isOnGround() const {
        return onGround;
    }

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

    [[nodiscard]] bool canWallJump() const;

    /**
     * Sets \code canJumpDueToGrace\endcode flag to false. Must only be called at the frame the player jumps.
     */
    void justJumped();

    [[nodiscard]] bool canDash() const {
        return dashCapacity;
    }

    [[nodiscard]] std::optional<std::reference_wrapper<CollidableObject>> isClimbing() const {
        return climbing;
    }

    void tryJumpInFuture(float seconds) {
        std::function<void()> cb = [this]{ tryJump(); };
        ScheduledEvent event = {cb, 0, true, 0, 0.2};
        Scheduler::getInstance().schedule(event);
    }

    bool tryJump() {
        if (canJump()) {
            return false;
        }
        // TODO

        return true;
    }

    bool tryDash(DashDirection direction) {
        if (!canDash()) {
            return false;
        }
        // TODO - Change hair color to indicate cannot dash

        // TODO - Dash Freeze

        auto vector = directionToVector(direction);
        auto dash_velocity = vector * DASH_SPEED;
        intrinsic_velocity = (intrinsic_velocity.length() > dash_velocity.length()) ? intrinsic_velocity : dash_velocity;

        return true;
    }

    bool tryClimb() {
        return false; // TODO
    }

private:
    PlayerSpriteHandler sprite_handler = PlayerSpriteHandler(sprite_state, sprite, facing);

    PlayerState* state = &PlayerStateGround::getInstance();

    bool onGround = true;

    bool dashCapacity = true;
    bool canJumpDueToGrace = false;

    // Also need to store information about which direction climbing which I guess is stored in facing
    std::optional<std::reference_wrapper<CollidableObject>> climbing = std::nullopt;

    void restoreStamina();
    void restoreDash();
};

#endif //PLAYER_H

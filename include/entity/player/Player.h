//
// Created by Agamjeet Singh on 13/07/25.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

#include "AbilityDash.h"
#include "AbilityJump.h"
#include "DashDirection.h"
#include "Facing.h"
#include "PlayerSpriteState.h"
#include "../CollidableObject.h"
#include "sprites/PlayerSpriteHandler.h"
#include "../../utility/Scheduler.h"
#include "events/Listener.h"
#include "events/PlayerLeftGround.h"
#include "physics/ContactsHandler.h"

class ContactsHandler;
struct PlayerOnGround;

class Player final : public CollidableObject {
public:
    Player(std::vector<sf::FloatRect> hitbox,
        sf::Sprite sprite,
        sf::Vector2f position = {0, 0});

    static constexpr float MAX_STAMINA = 110;
    static constexpr float JUMP_GRACE_COYOTE_TIME = 0.1; // In seconds
    static constexpr float JUMP_GRACE_BUFFER_TIME = 0.2; // In seconds
    static constexpr float VARIABLE_JUMP_TIME = 0.2; // In seconds
    static constexpr float WALK_SPEED = 90;
    static constexpr float RUN_REDUCE = 400;
    static constexpr float RUN_ACCELERATION = 1000;
    static constexpr float AIR_MULTIPLIER = 0.65;
    static constexpr float JUMP_SPEED = -105;
    static constexpr float JUMP_HORIZONTAL_BOOST = 40;
    static constexpr float HALF_GRAVITY_THRESHOLD = 40;
    static constexpr float UPWARD_CORNER_CORRECTION = 4;

    static constexpr float DASH_SPEED = 240;
    static constexpr float DASH_ATTACK_TIME = 0.1; // In seconds
    static constexpr float DASH_FREEZE_TIME = 0.05; // In seconds
    static constexpr float DASH_SPEED_TIME = 0.2; // In seconds
    static constexpr float TOTAL_DASH_TIME = DASH_FREEZE_TIME + DASH_SPEED_TIME;
    static constexpr float DASH_COOLDOWN_TIME = TOTAL_DASH_TIME; // In seconds, includes freeze time and dashing
    static constexpr float DASH_RESET_SPEED_HORIZONTAL = 160;
    static constexpr float DASH_RESET_SPEED_UP_DIAGONAL = 160;
    static constexpr float DASH_RESET_SPEED_VERTICAL = 120;
    static constexpr DashDirection DEFAULT_DASH_DIRECTION = DashDirection::RIGHT;
    static constexpr float DASH_FIRST_SNAPSHOT_TIME = 0.08;
    static constexpr float DASH_SECOND_SNAPSHOT_TIME = 0.15;
    static constexpr float DASH_THIRD_SNAPSHOT_TIME = 0.2;
    // Dash reset speed for down diagonal is the current speed (it is retained)

    static constexpr float GRAVITY = 900;

    float stamina = MAX_STAMINA;
    PlayerSpriteState sprite_state = PlayerSpriteState::GroundIdle;
    Facing facing = Facing::Right;

    void updateSprite(float deltaTime) {
        sprite_handler.update(deltaTime);
    }

    // ===== Gravity Movement Methods ====
    void enableGravity() {
        gravity_acceleration.y = GRAVITY;
    }

    void disableGravity() {
        gravity_acceleration.y = 0;
    }

    // ===== State Retrieval Methods =====

    [[nodiscard]] bool isOnGround() const {
        return onGround;
    }

    [[nodiscard]] bool canJump() const {
        return ability_jump.canPerform();
    }

    [[nodiscard]] bool canWallJump() const {
        return isClimbing().has_value(); // TODO
    }

    [[nodiscard]] bool canDash() const {
        return ability_dash.canPerform();
    }

    [[nodiscard]] std::optional<std::reference_wrapper<CollidableObject>> isClimbing() const {
        return climbing; // TODO
    }

    // ===== Try to Jump, Dash, Climb Methods =====

    // Jump buffering
    void tryJumpInFuture();

    bool tryJump();

    bool tryDash();

    bool tryClimb() {
        return false; // TODO
    }

    // ===== Abilities ====

    // Jump
    bool canJumpDueToCoyoteGrace = false;
    AbilityJump ability_jump{*this};

    // Dash
    bool dashCapacity = true;
    AbilityDash ability_dash{*this};

private:
    PlayerSpriteHandler sprite_handler = PlayerSpriteHandler(sprite_state, sprite, facing);

    Listener on_ground;

    Listener left_ground;

    Listener landed;

    bool onGround = false;

    // Also need to store information about which direction climbing which I guess is stored in facing
    std::optional<std::reference_wrapper<CollidableObject>> climbing = std::nullopt;

    void restoreStamina() {
        stamina = MAX_STAMINA;
    }
    void restoreDash() {
        dashCapacity = true;
    }
};

#endif //PLAYER_H

//
// Created by Agamjeet Singh on 13/07/25.
//

#include "entity/player/Player.h"

#include <utility>

#include "../../../include/entity/player/ability/AbilityDash.h"
#include "events/PlayerLanded.h"
#include "physics/ContactsHandler.h"
#include "events/PlayerOnGround.h"
#include "utility/EmptyTextures.h"

Player::Player(std::vector<sf::FloatRect> uncrouched_hitbox, std::vector<sf::FloatRect> crouched_hitbox,
               sf::Vector2f position) :
CollidableObject(uncrouched_hitbox, std::move(sf::Sprite{EmptyTextures::getInstance().getEmpty({32, 32})}), position),
on_ground(Listener::make_listener<PlayerOnGround>([this](const PlayerOnGround& event) {
    if (event.player != *this) return;
    onGround = true;
    if (!ability_dash.isPerforming()) {
        restoreDash();
    }
    restoreStamina();
}, ListenerPriority::HIGH)),
left_ground(Listener::make_listener<PlayerLeftGround>([this](const PlayerLeftGround& event) {
    if (event.player != *this) return;
    onGround = false;
    canJumpDueToCoyoteGrace = true;
    Scheduler& scheduler = Scheduler::getInstance();
    auto discard = scheduler.schedule([this](std::shared_ptr<ScheduledEvent> event, float deltaTime) { canJumpDueToCoyoteGrace = false; }, JUMP_GRACE_COYOTE_TIME);
}, ListenerPriority::HIGH)),
landed(Listener::make_listener<PlayerLanded>([this](const PlayerLanded& event) {
    if (event.player != *this) return;
    SoundManager::getInstance().play(SoundEffect::LAND);
    if (getTotalVelocity().y >= MAX_FALL) {
        squeeze({1.2, 0.8}, 0.05, 0.1);
    }
}, ListenerPriority::HIGH)), uncrouched_hitbox(std::move(uncrouched_hitbox)), crouched_hitbox(std::move(crouched_hitbox)) {
    gravity_acceleration.y = GRAVITY;
}

void Player::tryJumpInFuture() {
    auto discard = Scheduler::getInstance().schedule([this, time_elapsed = 0.f](std::shared_ptr<ScheduledEvent> event, float deltaTime) mutable {
        time_elapsed += deltaTime;
        if (time_elapsed >= JUMP_GRACE_BUFFER_TIME) {
            event->cancel();
        }
        tryJump();
    }, 0, true, 0);
}


bool Player::tryJump() {
    if (!ability_jump.canPerform()) {
        return false;
    }
    ability_jump.perform();
    return true;
}


bool Player::tryDash() {
    if (!ability_dash.canPerform()) {
        return false;
    }
    ability_dash.perform();
    return true;
}


void Player::kill() {
    if (dying) return;
    SoundManager::getInstance().play(SoundEffect::DEATH);
    sprite_state = PlayerSpriteState::Dead;
    disableGravity();
    friction_velocity = {0, 0};
    base_velocity = {-10, -10};
    dying = true;
    auto discard = Scheduler::getInstance().schedule([this](const std::shared_ptr<ScheduledEvent>& event, float dt) {
        sprite_state = PlayerSpriteState::GroundIdle;
        setPosition(respawn_position);
        base_velocity = {0, 0};
        friction_velocity = {0, 0};
        enableGravity();
        dying = false;
        restoreDash();
        restoreStamina();
    }, sprite_handler.getAnimationLength(PlayerSpriteState::Dead));
}


bool Player::canCollideWith(const CollidableObject &, Collision collision) const {
    return !dying;
}

void Player::crouch() {
    if (crouching) return;
    sprite_state = PlayerSpriteState::Ducking;
    crouching = true;
    hitbox.setRects(crouched_hitbox);
    squeeze({1.4, 0.7}, 0.03, 0.05);
}

void Player::uncrouch() {
    if (!crouching) return;
    crouching = false;
    hitbox.setRects(uncrouched_hitbox);
    squeeze({0.8, 1.2}, 0.015, 0.05);
}






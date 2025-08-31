//
// Created by Agamjeet Singh on 13/07/25.
//

#include "entity/player/Player.h"

#include "entity/player/AbilityDash.h"
#include "entity/player/PlayerStateClimbing.h"
#include "entity/player/PlayerStateAir.h"
#include "events/PlayerLanded.h"
#include "physics/ContactsHandler.h"
#include "events/PlayerOnGround.h"

Player::Player(std::vector<sf::FloatRect> hitbox,
               sf::Sprite sprite,
               sf::Vector2f position) :
CollidableObject(std::move(hitbox), std::move(sprite), position),
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
    SoundManager::getInstance().play(SoundEffect::LAND);
}, ListenerPriority::HIGH)) {
    gravity_acceleration.y = GRAVITY;
}

void Player::tryJumpInFuture() {
    auto cb = [this](std::shared_ptr<ScheduledEvent> event, float deltaTime){ tryJump(); };
    ScheduledEvent event = {std::move(cb), 0, true, 0, JUMP_GRACE_BUFFER_TIME};
    auto discard = Scheduler::getInstance().schedule(event);
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



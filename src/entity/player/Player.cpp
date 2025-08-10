//
// Created by Agamjeet Singh on 13/07/25.
//

#include "../../../include/entity/player/Player.h"

#include "../../../include/entity/player/PlayerStateClimbing.h"
#include "../../../include/entity/player/PlayerStateAir.h"
#include "../../../include/physics/ContactsHandler.h"

Player::Player(std::vector<sf::FloatRect> hitbox,
               sf::Sprite sprite,
               sf::Vector2f position) :
CollidableObject(std::move(hitbox), std::move(sprite), position) {}

void Player::updateSprite(float deltaTime) {
    sprite_handler.update(deltaTime);
}

void Player::restoreStamina() {
    stamina = MAX_STAMINA;
}

void Player::restoreDash() {
    dashCapacity = true;
}

void Player::onLand() {
    onGround = true;
    restoreDash();
    restoreStamina();
}

void Player::noLongerOnGround() {
    onGround = false;
    canJumpDueToGrace = true;
    Scheduler& scheduler = Scheduler::getInstance();
    scheduler.schedule([this] { canJumpDueToGrace = false; }, JUMP_GRACE_COYOTE_TIME);
}

[[nodiscard]] bool Player::canJump() const {
    return onGround || canJumpDueToGrace;
}

bool Player::canWallJump() const {
    return isClimbing().has_value();
}

void Player::justJumped() {
    canJumpDueToGrace = false;
}

void Player::changeStateTo(PlayerState& new_state) {
    state->onExit(*this);
    state = &new_state;
    state->onEntry(*this);
}

void Player::runDuringState() {
    state->duringState(*this);
}

PlayerState &Player::getState() const {
    return *state;
}

void Player::updateState() {
    if (state == &PlayerStateClimbing::getInstance()) {
        return;
    }
    if (state == &PlayerStateGround::getInstance()) {
        if (ContactsHandler::getInstance().onLand(*this)) {
            return;
        } else {
            changeStateTo(PlayerStateAir::getInstance());
        }
    }
    if (state == &PlayerStateAir::getInstance()) {
        if (ContactsHandler::getInstance().onLand(*this)) {
            changeStateTo(PlayerStateGround::getInstance());
        } else {
            return;
        }
    }
}


//
// Created by Agamjeet Singh on 13/07/25.
//

#include "../../../include/entity/player/Player.h"
#include "../../../include/physics/ContactsHandler.h"

Player::Player(std::vector<sf::FloatRect> hitbox,
               sf::Sprite sprite,
               sf::Vector2f position) :
CollidableObject(std::move(hitbox), std::move(sprite), position) {}

void Player::flipDirection() {
    facing = (facing == Facing::Right) ? Facing::Left : Facing::Right;
}

void Player::updates(float deltaTime) {
    updatePhysicsStates();

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
    scheduler.schedule([this] { canJumpDueToGrace = false; }, JUMP_GRACE_TIME);
}

[[nodiscard]] bool Player::canJump() const {
    return onGround || canJumpDueToGrace;
}

void Player::justJumped() {
    canJumpDueToGrace = false;
}

void Player::updatePhysicsStates() {
    // On ground
    if (onGround == true && !ContactsHandler::getInstance().onLand(*this)) {
        noLongerOnGround();
    }
    onGround = ContactsHandler::getInstance().onLand(*this);


    // TODO
}

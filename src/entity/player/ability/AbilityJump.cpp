//
// Created by Agamjeet Singh on 16/08/25.
//

#include "../../../../include/entity/player/ability/AbilityJump.h"
#include "../../../../include/entity/player/Player.h"
#include "../../../../include/utility/Scheduler.h"

bool AbilityJump::canPerform() const {
    return !performing && !onCooldown && (player.isOnGround() || player.canJumpDueToCoyoteGrace) && !player.dying;
}

void AbilityJump::perform() {
    performing = true;
    player.canJumpDueToCoyoteGrace = false;
    variable_jump = true;
    player.disableGravity();
    variable_jump_timer = Scheduler::getInstance().schedule([this](std::shared_ptr<ScheduledEvent> event, float deltaTime) { endVariableJump(); }, Player::VARIABLE_JUMP_TIME);
    call_during = Scheduler::getInstance().schedule({[this](std::shared_ptr<ScheduledEvent> event, float deltaTime){ if (this->isPerforming()) { this->callDuring(); } }, 0, true, 0});

    player.base_velocity.y = Player::JUMP_SPEED;
    float direction = 0;
    if (InputManager::getInstance().isPressed(moveRight) && (!InputManager::getInstance().isPressed(moveLeft) || InputManager::getInstance().wasPressedEarlierThan(moveLeft, moveRight))) {
        direction = 1;
    }
    if (InputManager::getInstance().isPressed(moveLeft) && (!InputManager::getInstance().isPressed(moveRight) || InputManager::getInstance().wasPressedEarlierThan(moveRight, moveLeft))) {
        direction = -1;
    }
    player.base_velocity.x += direction * Player::JUMP_HORIZONTAL_BOOST;

    player.squeeze({0.7, 1.4}, 0.05, 0.2);
}

void AbilityJump::cancel() {
    endVariableJump();
    if (variable_jump_timer) variable_jump_timer->cancelled = true;
    if (call_during) call_during->cancelled = true;
}

void AbilityJump::callDuring() {
    if (!variable_jump) return;

    if (!InputManager::getInstance().isPressed(jumpKey)) {
        endVariableJump();
        if (variable_jump_timer) variable_jump_timer->cancelled = true;
        if (call_during) call_during->cancelled = true;
    } else {
        player.base_velocity.y = Player::JUMP_SPEED;
    }
}

void AbilityJump::endVariableJump() {

    variable_jump = false;
    player.enableGravity();
    performing = false;
}


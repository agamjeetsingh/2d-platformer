//
// Created by Agamjeet Singh on 16/08/25.
//

#ifndef ABILITYJUMP_H
#define ABILITYJUMP_H
#include "Ability.h"
#include "utility/InputManager.h"
#include "utility/ScheduledEvent.h"

/**
 * The notion of \code performing\endcode is not very strong with jumping. It is true if and only if the jump
 * button has been pressed every frame since the start of the jump, the variable jump timer has not ended yet, and
 * the jump event has not been cancelled. In other words, performing is true between the first frame of a jump and the
 * earliest of:
 *
 * - the jump key being released,
 *
 * - the variable jump timer ending,
 *
 * - the jump being cancelled by external events.
 */
class AbilityJump final : public Ability {
public:
    explicit AbilityJump(Player& player): Ability(player) {}

    /**
     * @brief A player is allowed to jump when they are on the ground, or they can jump due to Coyote Grace which is
     * true for \code Player::JUMP_GRACE_COYOTE_TIME\endcode seconds after leaving the ground if the player has not
     * jumped yet.
     * @return whether the player is allowed to jump.
     */
    [[nodiscard]] bool canPerform() const override;

    /**
     * @brief Performs the jump. Sets \code canJumpDueToCoyoteGrace\endcode to false to avoid double jumps. Starts the
     * variable jump timer (for \code Player::VARIABLE_JUMP_TIME\endcode seconds) and sets the
     * \code variable_jump\endcode tag (used by \code callDuring()\endcode) to true. Disables gravity for the duration
     * of the variable jump. After the variable jump timer is over (assuming it was not cancelled), it would re-enable
     * gravity and sets the \code variable_jump\endcode tag to false.
     *
     * Sets the player's intrinsic y velocity to \code Player::JUMP_SPEED\endcode and gives them a horizontal boost of
     * \code Player::JUMP_HORIZONTAL_BOOST\endcode if they are trying to move left or right.
     */
    void perform() override;

    /**
     * @brief Cancels the jump event and performs clean-up. Should be called when the player bumps their head on a
     * ceiling or starts climbing or dashing. Sets the \code variable_jump\endcode tag (used by
     * \code callDuring()\endcode) to false. Re-enables gravity and cancels the variable jump timer task.
     */
    void cancel() override;

private:
    std::shared_ptr<ScheduledEvent> variable_jump_timer;
    std::shared_ptr<ScheduledEvent> call_during;
    bool variable_jump = false;
    const sf::Keyboard::Key jumpKey = sf::Keyboard::Key::Space;
    const sf::Keyboard::Key moveLeft = sf::Keyboard::Key::A;
    const sf::Keyboard::Key moveRight = sf::Keyboard::Key::D;

    /**
     * @brief If we are still inside the variable jump, then if the \code jumpKey\endcode is pressed, sets the player's
     * intrinsic y velocity to \code Player::JUMP_SPEED\endcode. If it is not pressed, then ends the variable jump by
     * setting the \code variable_jump\endcode tag to false, enabling gravity and cancelling the
     * \code variable_jump_timer\endcode.
     */
    void callDuring() override;

    void startCooldown() override { /** No Cooldown */ }

    void endVariableJump();
};



#endif //ABILITYJUMP_H

//
// Created by Agamjeet Singh on 26/07/25.
//

#ifndef PLAYERINPUTHANDLER_H
#define PLAYERINPUTHANDLER_H

#include "Player.h"
#include "../../utility/InputManager.h"
#include <SFML/Graphics.hpp>

#include "DashDirection.h"

using namespace sf::Keyboard;

class PlayerInputHandler {
public:
    explicit PlayerInputHandler(Player& player): player(player) {}

    /**
     * @brief Updates the player by deltaTime. Routes to the individual update functions for each of the three states.
     *
     * May change player's \code velocity\endcode, \code facing\endcode, and \code state\endcode via
     * \code Player::try*\endcode methods.
     *
     * @param deltaTime The time to update the player by according to the input.
     */
    void update(float deltaTime);

    /**
     * Gets the dash direction according to the current input. Combines the latest pressed vertical key (if any) and
     * the latest pressed horizontal key (if any) to get the dash direction. Defaults to
     * \code Player::DEFAULT_DASH_DIRECTION\endcode no horizontal or vertical key is pressed.
     * @return The dash direction according to the current input.
     */
    static DashDirection getDashDirection();

private:
    Player& player;

    static bool isPressed(const Key key) {
        return InputManager::getInstance().isPressed(key);
    }

    static bool wasPressedEarlierThan(const Key key1, const Key key2) {
        return InputManager::getInstance().wasPressedEarlierThan(key1, key2);
    }

    void handleLeftRightMovement(float deltaTime);

    /**
     * @brief Helper math function that makes a variable approach a value from either (positive or negative) direction
     * by the specified step. Clamps \code to_make_approach\endcode to \code to_approach\endcode if over-stepping.
     * @param to_make_approach The reference that will approach \code to_approach\endcode by \code step\endcode.
     * @param to_approach The value \code to_make_approach\endcode will approach.
     * @param step The rate at which \code to_make_approach\endcode will try to approach \code to_approach\endcode.
     */
    static void approach(float& to_make_approach, float to_approach, float step);

    static constexpr Key dashKey = Key::L;
    static constexpr Key climbKey = Key::K;
    static constexpr Key moveLeft = Key::A;
    static constexpr Key moveRight = Key::D;
    static constexpr Key moveDown = Key::S;
    static constexpr Key moveUp = Key::W;
    static constexpr Key crouchKey = Key::S;
    static constexpr Key jumpKey = Key::Space;
};



#endif //PLAYERINPUTHANDLER_H

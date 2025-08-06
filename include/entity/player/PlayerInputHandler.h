//
// Created by Agamjeet Singh on 26/07/25.
//

#ifndef PLAYERINPUTHANDLER_H
#define PLAYERINPUTHANDLER_H

#include "Player.h"
#include "../../utility/InputManager.h"
#include <SFML/Graphics.hpp>

using namespace sf::Keyboard;

class PlayerInputHandler {
public:
    explicit PlayerInputHandler(Player& player): player(player) {}

    void update() {
        if (isPressed(dashKey)) {
            tryDash();
        }

        if (isPressed(moveLeft) && (!isPressed(moveRight) || wasPressedEarlierThan(moveRight, moveLeft))) {
            // Move left
            player.facing = Facing::Left;
            player.intrinsic_velocity.x = -Player::WALK_SPEED;
        }

        if (isPressed(moveRight) && (!isPressed(moveLeft) || wasPressedEarlierThan(moveLeft, moveRight))) {
            // Move right
            player.facing = Facing::Right;
            player.intrinsic_velocity.x = Player::WALK_SPEED;
        }

        if (!isPressed(moveLeft) && !isPressed(moveRight)) {
            player.intrinsic_velocity.x = 0;
        }

        if (isPressed(jumpKey)) {
            tryJump();
        }

        if (isPressed(crouchKey)) {
            // TODO - Crouch
        }
    }

private:
    Player& player;

    static bool isPressed(const Key key) {
        return InputManager::getInstance().isPressed(key);
    }

    static bool wasPressedEarlierThan(const Key key1, const Key key2) {
        return InputManager::getInstance().wasPressedEarlierThan(key1, key2);
    }

    Key dashKey = Key::L;
    Key climbKey = Key::K;
    Key moveLeft = Key::A;
    Key moveRight = Key::D;
    Key moveDown = Key::S;
    Key moveUp = Key::W;
    Key crouchKey = Key::S;
    Key jumpKey = Key::Space;

    void tryDash() {
        if (!player.canDash()) {
            return;
        }
        // TODO - Change hair color to indicate cannot dash
    }

    void tryJump() {
        if (!player.canJump()) {
            return;
        }
        // TODO
    }
};



#endif //PLAYERINPUTHANDLER_H

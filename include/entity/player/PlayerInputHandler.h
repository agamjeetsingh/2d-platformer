//
// Created by Agamjeet Singh on 26/07/25.
//

#ifndef PLAYERINPUTHANDLER_H
#define PLAYERINPUTHANDLER_H

#include <ranges>

#include "Player.h"
#include "../../utility/InputManager.h"
#include <SFML/Graphics.hpp>

#include "DashDirection.h"
#include "PlayerStateAir.h"
#include "PlayerStateClimbing.h"

using namespace sf::Keyboard;

class PlayerInputHandler {
public:
    explicit PlayerInputHandler(Player& player): player(player) {}

    void update(float deltaTime) {
        if (&player.getState() == &PlayerStateGround::getInstance()) {
            updateFromGroundState(deltaTime);
            return;
        }

        if (&player.getState() == &PlayerStateAir::getInstance()) {
            updateFromAirState(deltaTime);
            return;
        }

        if (&player.getState() == &PlayerStateClimbing::getInstance()) {
            updateFromClimbState(deltaTime);
            return;
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

    void updateFromGroundState(float deltaTime) {
        assert(&player.getState() == &PlayerStateGround::getInstance());

        if (isPressed(dashKey)) {
            if (bool success = player.tryDash(getDashDirection())) return;
        }

        if (isPressed(climbKey)) {
            if (bool success = player.tryClimb()) return;
        }

        if (isPressed(jumpKey)) {
            if (bool success = player.tryJump()) return;
        }

        bool moveSomewhere = false;

        if (isPressed(moveLeft) && (!isPressed(moveRight) || wasPressedEarlierThan(moveRight, moveLeft))) {
            // Move left
            moveSomewhere = true;
            player.facing = Facing::Left;
            approach(player.intrinsic_velocity.x, -Player::WALK_SPEED, Player::RUN_ACCELERATION * deltaTime);
        }

        if (isPressed(moveRight) && (!isPressed(moveLeft) || wasPressedEarlierThan(moveLeft, moveRight))) {
            // Move right
            moveSomewhere = true;
            player.facing = Facing::Right;
            approach(player.intrinsic_velocity.x, Player::WALK_SPEED, Player::RUN_ACCELERATION * deltaTime);
        }

        if (!moveSomewhere) {
            approach(player.intrinsic_velocity.x, 0, Player::RUN_ACCELERATION * deltaTime);
        }
    }

    void updateFromAirState(float deltaTime) {
        assert(&player.getState() == &PlayerStateAir::getInstance());
    }

    void updateFromClimbState(float deltaTime) {
        assert(&player.getState() == &PlayerStateClimbing::getInstance());
    }

    static void approach(float& to_make_approach, float to_approach, float step) {
        assert(step >= 0);
        if (to_make_approach > to_approach) {
            to_make_approach = std::max(to_make_approach - step, to_approach);
        } else if (to_make_approach < to_approach) {
            to_make_approach = std::min(to_make_approach + step, to_approach);
        }
    }

    DashDirection getDashDirection() {
        std::optional<Key> verticalKey;
        std::optional<Key> horizontalKey;

        if (isPressed(moveUp)) {
            if (isPressed(moveDown)) {
                verticalKey = wasPressedEarlierThan(moveUp, moveDown) ? moveDown : moveUp;
            } else {
                verticalKey = moveUp;
            }
        } else {
            if (isPressed(moveDown)) {
                verticalKey = moveDown;
            }
        }

        if (isPressed(moveLeft)) {
            if (isPressed(moveRight)) {
                horizontalKey = wasPressedEarlierThan(moveLeft, moveRight) ? moveRight : moveLeft;
            } else {
                horizontalKey = moveLeft;
            }
        } else {
            if (isPressed(moveRight)) {
                horizontalKey = moveRight;
            }
        }

        DashDirection direction = Player::DEFAULT_DASH_DIRECTION;

        if (verticalKey) {
            if (horizontalKey) {
                if (verticalKey == moveUp && horizontalKey == moveLeft) {
                    direction = DashDirection::UP_LEFT;
                }
                if (verticalKey == moveUp && horizontalKey == moveRight) {
                    direction = DashDirection::UP_RIGHT;
                }
                if (verticalKey == moveDown && horizontalKey == moveLeft) {
                    direction = DashDirection::DOWN_LEFT;
                }
                if (verticalKey == moveDown && horizontalKey == moveRight) {
                    direction = DashDirection::DOWN_RIGHT;
                }
            } else {
                direction = verticalKey == moveUp ? DashDirection::UP : DashDirection::DOWN;
            }
        } else {
            if (horizontalKey) {
                direction = horizontalKey == moveLeft ? DashDirection::LEFT : DashDirection::RIGHT;
            }
        }

        return direction;
    }

    Key dashKey = Key::L;
    Key climbKey = Key::K;
    Key moveLeft = Key::A;
    Key moveRight = Key::D;
    Key moveDown = Key::S;
    Key moveUp = Key::W;
    Key crouchKey = Key::S;
    Key jumpKey = Key::Space;
};



#endif //PLAYERINPUTHANDLER_H

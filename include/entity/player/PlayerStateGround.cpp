//
// Created by Agamjeet Singh on 08/08/25.
//

#include "PlayerStateGround.h"
#include "Player.h"

PlayerStateGround &PlayerStateGround::getInstance() {
    static PlayerStateGround instance;
    return instance;
}

void PlayerStateGround::onEntry(Player &player)  {
    player.onLand();
}

void PlayerStateGround::duringState(Player &player) {
    player.onLand();
}

void PlayerStateGround::onExit(Player &player) {
    player.noLongerOnGround();
}

PlayerStateGround::PlayerStateGround(): PlayerState(onEntry, duringState, onExit) {}




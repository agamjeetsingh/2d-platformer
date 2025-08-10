//
// Created by Agamjeet Singh on 08/08/25.
//

#ifndef PLAYERSTATEGROUND_H
#define PLAYERSTATEGROUND_H
#include "PlayerState.h"

class Player;

class PlayerStateGround: public PlayerState {
public:
    PlayerStateGround(const PlayerStateGround&) = delete;
    PlayerStateGround& operator=(const PlayerStateGround&) = delete;

    static PlayerStateGround& getInstance();

private:
    static void onEntry(Player& player);

    static void duringState(Player& player);

    static void onExit(Player& player);

    PlayerStateGround();
};



#endif //PLAYERSTATEGROUND_H

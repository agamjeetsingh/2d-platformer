//
// Created by Agamjeet Singh on 08/08/25.
//

#ifndef PLAYERSTATEAIR_H
#define PLAYERSTATEAIR_H
#include "Player.h"
#include "PlayerState.h"


class PlayerStateAir: public PlayerState {
public:
    PlayerStateAir(const PlayerStateAir&) = delete;
    PlayerStateAir& operator=(const PlayerStateAir&) = delete;

    static PlayerStateAir& getInstance() {
        static PlayerStateAir instance;
        return instance;
    }
private:
    static void onEntry(Player& player) {
    }

    static void duringState(Player& player) {
    }

    static void onExit(Player& player) {
    }

    PlayerStateAir(): PlayerState(onEntry, duringState, onExit) {}
};



#endif //PLAYERSTATEAIR_H

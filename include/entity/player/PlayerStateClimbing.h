//
// Created by Agamjeet Singh on 08/08/25.
//

#ifndef PLAYERSTATECLIMBING_H
#define PLAYERSTATECLIMBING_H
#include "PlayerState.h"


class PlayerStateClimbing : public PlayerState {
public:
    PlayerStateClimbing(const PlayerStateClimbing&) = delete;
    PlayerStateClimbing& operator=(const PlayerStateClimbing&) = delete;

    static PlayerStateClimbing& getInstance() {
        static PlayerStateClimbing instance;
        return instance;
    }

private:
    static void onEntry(Player& player) {
    }

    static void duringState(Player& player) {
    }

    static void onExit(Player& player) {
    }

    PlayerStateClimbing(): PlayerState(onEntry, duringState, onExit) {}
};



#endif //PLAYERSTATECLIMBING_H

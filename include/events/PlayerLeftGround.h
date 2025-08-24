//
// Created by Agamjeet Singh on 19/08/25.
//

#ifndef PLAYERLEFTGROUND_H
#define PLAYERLEFTGROUND_H
#include "entity/player/Player.h"


struct PlayerLeftGround {
    explicit PlayerLeftGround(const Player& player): player(player) {}

    const Player& player;
    // TODO - Implement which ground it was on, if needed
};



#endif //PLAYERLEFTGROUND_H

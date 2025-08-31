//
// Created by Agamjeet Singh on 26/08/25.
//

#ifndef PLAYERLANDED_H
#define PLAYERLANDED_H
#include "entity/player/Player.h"


struct PlayerLanded {
    PlayerLanded(const Player& player, Contact contact): player(player), contact(contact) {}

    const Player& player;
    Contact contact;
};



#endif //PLAYERLANDED_H

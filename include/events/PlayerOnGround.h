//
// Created by Agamjeet Singh on 19/08/25.
//

#ifndef PLAYERONGROUND_H
#define PLAYERONGROUND_H
#include "Contact.h"
#include "entity/CollidableObject.h"
#include "../entity/player/Player.h"


struct PlayerOnGround {
    explicit PlayerOnGround(Contact contact);

    CollidableObject& surface;
    Player& player;
    sf::FloatRect surfaceRect;
    sf::FloatRect playerRect;
};



#endif //PLAYERONGROUND_H

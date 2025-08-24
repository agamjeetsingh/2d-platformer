//
// Created by Agamjeet Singh on 19/08/25.
//

#include "../../include/events/PlayerOnGround.h"

PlayerOnGround::PlayerOnGround(Contact contact) : surface(contact.objectA.isPlayer() ? contact.objectB : contact.objectA),
    player(contact.objectA.isPlayer() ? *contact.objectA.isPlayer() : *contact.objectB.isPlayer()),
    surfaceRect(contact.objectA.isPlayer() ? contact.getCollidingRectB() : contact.getCollidingRectA()),
    playerRect(contact.objectA.isPlayer() ? contact.getCollidingRectA() : contact.getCollidingRectB()) {}

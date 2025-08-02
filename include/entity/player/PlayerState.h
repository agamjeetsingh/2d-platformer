//
// Created by Agamjeet Singh on 25/07/25.
//

#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H



enum class PlayerState {
    Idle, // On ground, not moving
    Walking, // On ground, moving
    Climbing,
    Dash,
    Falling,
};



#endif //PLAYERSTATE_H

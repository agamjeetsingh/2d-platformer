//
// Created by Agamjeet Singh on 08/08/25.
//

#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H
#include <functional>

class Player;

class PlayerState {
public:
    const std::function<void(Player&)> onEntry; // To be called when entering this state
    const std::function<void(Player&)> duringState; // To be called every frame we are in this state, excluding the one we enter the state
    const std::function<void(Player&)> onExit; // To be called when exiting this state

protected:
    PlayerState(std::function<void(Player&)> onEntry, std::function<void(Player&)> duringState, std::function<void(Player&)> onExit);
};



#endif //PLAYERSTATE_H

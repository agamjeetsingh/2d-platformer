//
// Created by Agamjeet Singh on 08/08/25.
//

#include "../../../include/entity/player/PlayerState.h"

PlayerState::PlayerState(std::function<void(Player&)> onEntry, std::function<void(Player&)> duringState, std::function<void(Player&)> onExit):
    onEntry(std::move(onEntry)), duringState(std::move(duringState)), onExit(std::move(onExit)) {}

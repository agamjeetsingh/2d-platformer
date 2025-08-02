//
// Created by Agamjeet Singh on 28/07/25.
//

#ifndef GLOBALISLAND_H
#define GLOBALISLAND_H
#include "Island.h"

class CollidableObject;

class GlobalIsland : public Island {
public:
    using Island::leader;

    GlobalIsland();

    static std::shared_ptr<Island> getInstance();
};


#endif //GLOBALISLAND_H

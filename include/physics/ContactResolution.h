//
// Created by Agamjeet Singh on 24/07/25.
//

#ifndef CONTACTRESOLUTION_H
#define CONTACTRESOLUTION_H

#include "../entity/CollidableObject.h"
#include "../entity/CollidableObjectType.h"
#include "../events/Collision.h"

class ContactResolution {
public:
    static void resolve(Collision collision);
};



#endif //CONTACTRESOLUTION_H

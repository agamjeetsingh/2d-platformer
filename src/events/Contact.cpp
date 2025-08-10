//
// Created by Agamjeet Singh on 27/07/25.
//

#include "../../include/events/Contact.h"

bool Contact::operator==(const Contact &other) const {
    return &objectA == &other.objectA &&
           &objectB == &other.objectB &&
           collidingRectA == other.collidingRectA &&
           collidingRectB == other.collidingRectB &&
           axis == other.axis;
}

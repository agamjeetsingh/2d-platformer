//
// Created by Agamjeet Singh on 17/08/25.
//

#include "../../include/events/Listener.h"


void Listener::call(const void *ev) const {
    erased_cb(ev);
}




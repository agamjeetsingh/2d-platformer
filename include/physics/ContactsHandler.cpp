//
// Created by Agamjeet Singh on 27/07/25.
//

#include "ContactsHandler.h"
#include "../entity/player/Player.h"

void ContactsHandler::updateIslands() const {
    for (const auto& object: CollisionsHandler::getInstance().getBodies()) {
        if (object.get().type == CollidableObjectType::Immovable) {
            continue;
        }

        auto resting_contacts = restingOnSurfaces(object);

        // Check if object is player and is climbing:
        auto* player = dynamic_cast<Player*>(&object.get());
        if (player && player->isClimbing()) {
            auto climbing = player->isClimbing().value();
            player->joinIsland(climbing.get().getIsland());
            continue;
        }

        if (resting_contacts.empty()) {
            object.get().joinIsland(GlobalIsland::getInstance());
            continue;
        }

        float max_contact_length = resting_contacts[0].contactLength();
        int best_contact_index = 0;
        // Contact copy constructor is implicitly deleted because it contains references
        for (int i = 0; i < resting_contacts.size(); i++) {
            if (resting_contacts[i].contactLength() > max_contact_length) {
                max_contact_length = resting_contacts[i].contactLength();
                best_contact_index = i;
            } else if (resting_contacts[i].contactLength() == max_contact_length) {
                // TODO - Find a uniform ordering
            }
        }
        // Should only be moved by immovables?
        Contact best_contact = resting_contacts[best_contact_index];
        CollidableObject& other_object = (&best_contact.objectA == &object.get()) ? best_contact.objectB : best_contact.objectA;
        object.get().joinIsland(other_object.getIsland());

        std::cout << "--------------------" << std::endl;
        std::cout << "Island Info on Updation" << std::endl;
        std::cout << "Global Island size: " << GlobalIsland::getInstance()->getMembers().size() << std::endl;
        std::cout << "--------------------" << std::endl;
    }
}


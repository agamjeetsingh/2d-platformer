//
// Created by Agamjeet Singh on 27/07/25.
//

#include "physics/ContactsHandler.h"
#include "entity/player/Player.h"
#include "events/EventBus.h"
#include "events/PlayerLanded.h"
#include "events/PlayerLeftGround.h"
#include "events/PlayerOnGround.h"

ContactsHandler &ContactsHandler::getInstance() {
    static ContactsHandler instance;
    return instance;
}

void ContactsHandler::addContact(Contact contact) {
    auto keyA = std::ref(contact.objectA);
    auto keyB = std::ref(contact.objectB);
    contacts[keyA].push_back(contact);
    if (contact.objectA != contact.objectB) {
        contacts[keyB].push_back(contact);
    }
    contacts_vector.push_back(contact);

    if ((contact.objectA.isPlayer() || contact.objectB.isPlayer()) && contact.axis == ContactAxis::Y) {
        const CollidableObject* player = contact.objectA.isPlayer() ? &contact.objectA : &contact.objectB;
        const CollidableObject* other = contact.objectA.isPlayer() ? &contact.objectB : &contact.objectA;

        bool playerAboveOther = contact.objectA.isPlayer() ?
            contact.getCollidingRectA().position.y < contact.getCollidingRectB().position.y :
            contact.getCollidingRectB().position.y < contact.getCollidingRectA().position.y;
            
        if (playerAboveOther) {
            EventBus::getInstance().emit(PlayerOnGround{contact}, EventExecuteTime::POST_PHYSICS);
        }
    }
    EventBus::getInstance().emit(contact, EventExecuteTime::POST_PHYSICS);
}

void ContactsHandler::newFrame() {
    previous_frame_contacts = std::move(contacts);
    contacts = decltype(contacts){};
    contacts_vector.clear();
}

std::vector<Contact> ContactsHandler::allContacts(const CollidableObject &object) const {
    const auto it = contacts.find(object);
    return it == contacts.end() ? std::vector<Contact>{} : it->second;
}


bool ContactsHandler::onLand(const CollidableObject &object, bool previous_frame) const {
    auto& contacts = previous_frame ? previous_frame_contacts : this->contacts;
    if (!contacts.contains(object)) {
        return false;
    }
    const auto it = contacts.find(object);
    if (it == contacts.end()) {
        return false;
    }
    const std::vector<Contact>& all_contacts = it->second;

    return std::ranges::any_of(all_contacts, [&object](const Contact &contact) {
        return contact.axis == ContactAxis::Y && (object == contact.objectA
                                                      ? contact.getCollidingRectA().position.y < contact.
                                                        getCollidingRectB().position.y
                                                      : contact.getCollidingRectB().position.y < contact.
                                                        getCollidingRectA().position.y);
    });
}

std::vector<Contact> ContactsHandler::restingOnSurfaces(const CollidableObject &object, bool previous_frame) const {
    auto& contacts = previous_frame ? previous_frame_contacts : this->contacts;
    std::vector<Contact> horizontal_contacts;
    if (!contacts.contains(object)) {
        return {};
    }
    const std::vector<Contact>& all_contacts = contacts.at(object);
    auto pred = [&object](const Contact &contact) {
        return contact.axis == ContactAxis::Y && (object == contact.objectA
                                                      ? contact.getCollidingRectA().position.y < contact.
                                                        getCollidingRectB().position.y
                                                      : contact.getCollidingRectB().position.y < contact.
                                                        getCollidingRectA().position.y);
    };
    std::ranges::copy_if(all_contacts, std::back_inserter(horizontal_contacts), pred);
    return horizontal_contacts;
}

std::vector<CollidableObject> ContactsHandler::nextToVerticalSurfaces(const CollidableObject &object, bool previous_frame) const {
    auto& contacts = previous_frame ? previous_frame_contacts : this->contacts;
    std::vector<CollidableObject> vertical_surfaces;
    if (!contacts.contains(object)) {
        return {};
    }
    const auto it = contacts.find(object);
    if (it == contacts.end()) {
        return {};
    }
    const std::vector<Contact>& all_contacts = it->second;

    for (const auto &contact: all_contacts | std::views::filter([](const Contact &contact) {
        return contact.axis == ContactAxis::X;
    })) {
        auto other_object = (contact.objectA == object) ? contact.objectB : contact.objectA;
        vertical_surfaces.push_back(other_object);
    }
    return vertical_surfaces;
}

[[nodiscard]] std::vector<Contact> ContactsHandler::getContacts() const{
    return contacts_vector;
}

void ContactsHandler::emitPlayerEvents() const {
    EventBus& instance = EventBus::getInstance();
    for (const auto& object: std::views::keys(previous_frame_contacts)) {
        if (object.get().isPlayer() && onLand(object.get(), true) && !onLand(object.get())) {
            EventBus::getInstance().emit(PlayerLeftGround{*object.get().isPlayer()}, EventExecuteTime::POST_PHYSICS);
        }
    }
    for (const auto& object: std::views::keys(contacts)) {
        if (object.get().isPlayer() && !onLand(object.get(), true) && onLand(object.get())) {
            auto contacts = restingOnSurfaces(object.get());
            assert(!contacts.empty());
            EventBus::getInstance().emit(PlayerLanded{*object.get().isPlayer(), contacts[0]},
                                         EventExecuteTime::POST_PHYSICS);
        }
    }
}


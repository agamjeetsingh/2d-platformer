//
// Created by Agamjeet Singh on 27/07/25.
//

#include "physics/ContactsHandler.h"
#include "entity/player/Player.h"

ContactsHandler &ContactsHandler::getInstance() {
    static ContactsHandler instance;
    return instance;
}

void ContactsHandler::addContact(Contact contact) {
    auto key = std::make_pair(std::ref(contact.objectA), std::ref(contact.objectB));
    contacts.erase(key);
    contacts.emplace(key, contact);
    if (contact.objectA != contact.objectB) {
        contacts.erase({contact.objectB, contact.objectA});
    }
}

void ContactsHandler::removeContact(Contact contact) {
    contacts.erase({std::ref(contact.objectA), std::ref(contact.objectB)});
    contacts.erase({std::ref(contact.objectB), std::ref(contact.objectA)});
}

void ContactsHandler::reset() {
    contacts.clear();
}

std::optional<Contact> ContactsHandler::areTouching(CollidableObject &objectA, CollidableObject &objectB) const {
    auto key1 = std::make_pair(std::ref(objectA), std::ref(objectB));
    auto key2 = std::make_pair(std::ref(objectB), std::ref(objectA));

    if (contacts.contains(key1)) {
        return contacts.at(key1);
    } else if (contacts.contains(key2)) {
        return contacts.at(key2);
    } else {
        return std::nullopt;
    }
}

std::vector<Contact> ContactsHandler::allContacts(const CollidableObject &object) const {
    std::vector<Contact> object_contacts;
    for (const auto& keyValue: contacts) {
        const auto [fst, snd] = keyValue.first;
        if (fst.get() == object || snd.get() == object) {
            object_contacts.push_back(keyValue.second);
        }
    }
    return object_contacts;
}

bool ContactsHandler::onLand(const CollidableObject &object) const {
    auto all_contacts = allContacts(object);
    for (const auto& contact: all_contacts) {
        if (contact.axis == ContactAxis::Y) {
            return true;
        }
    }
    return false;
}

std::vector<CollidableObject> ContactsHandler::nextToVerticalSurfaces(const CollidableObject &object) const {
    std::vector<CollidableObject> vertical_surfaces;
    auto all_contacts = allContacts(object);
    for (const auto& contact: all_contacts) {
        if (contact.axis == ContactAxis::X) {
            auto other_object = (contact.objectA == object) ? contact.objectB : contact.objectA;
            vertical_surfaces.push_back(other_object);
        }
    }
    return vertical_surfaces;
}

std::vector<Contact> ContactsHandler::restingOnSurfaces(const CollidableObject &object) const {
    std::vector<Contact> horizontal_contacts;
    auto all_contacts = allContacts(object);
    for (const auto& contact: all_contacts) {
        if (contact.axis == ContactAxis::Y) {
            horizontal_contacts.push_back(contact);
        }
    }
    return horizontal_contacts;
}

[[nodiscard]] const std::unordered_map<std::pair<std::reference_wrapper<CollidableObject>, std::reference_wrapper<CollidableObject>>, Contact, ContactKeyHash, ContactKeyEqual>& ContactsHandler::getContacts() const{
    return contacts;
}

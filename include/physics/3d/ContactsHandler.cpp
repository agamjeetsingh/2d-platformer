//
// Created by Agamjeet Singh on 22/09/25.
//

#include "ContactsHandler.h"
#include "CollidableObject.h"

#include <ranges>
#include "events/EventBus.h"
#include "Contact.h"

eng::d3::ContactsHandler &eng::d3::ContactsHandler::getInstance() {
    static ContactsHandler instance;
    return instance;
}

void eng::d3::ContactsHandler::addContact(Contact contact) {
    auto keyA = std::ref(contact.objectA);
    auto keyB = std::ref(contact.objectB);
    contacts[keyA].push_back(contact);
    if (contact.objectA != contact.objectB) {
        contacts[keyB].push_back(contact);
    }
    contacts_vector.push_back(contact);

    EventBus::getInstance().emit(contact, EventExecuteTime::POST_PHYSICS);
}

void eng::d3::ContactsHandler::newFrame() {
    previous_frame_contacts = std::move(contacts);
    contacts = decltype(contacts){};
    contacts_vector.clear();
}

std::vector<eng::d3::Contact> eng::d3::ContactsHandler::allContacts(const CollidableObject &object) const {
    const auto it = contacts.find(object);
    return it == contacts.end() ? std::vector<Contact>{} : it->second;
}


bool eng::d3::ContactsHandler::onLand(const CollidableObject &object, bool previous_frame) const {
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
        return contact.axis.y != 0 && (object == contact.objectA
                                                      ? contact.getCollidingBoxA().position.y < contact.
                                                        getCollidingBoxB().position.y
                                                      : contact.getCollidingBoxB().position.y < contact.
                                                        getCollidingBoxA().position.y);
    });
}

std::vector<eng::d3::Contact> eng::d3::ContactsHandler::restingOnSurfaces(
    const CollidableObject &object, bool previous_frame) const {
    auto& contacts = previous_frame ? previous_frame_contacts : this->contacts;
    std::vector<Contact> horizontal_contacts;
    if (!contacts.contains(object)) {
        return {};
    }
    const std::vector<Contact>& all_contacts = contacts.at(object);
    auto pred = [&object](const Contact &contact) {
        return contact.axis.y != 0 && (object == contact.objectA
                                                      ? contact.getCollidingBoxA().position.y < contact.
                                                        getCollidingBoxB().position.y
                                                      : contact.getCollidingBoxB().position.y < contact.
                                                        getCollidingBoxA().position.y);
    };
    std::ranges::copy_if(all_contacts, std::back_inserter(horizontal_contacts), pred);
    return horizontal_contacts;
}

std::vector<eng::d3::CollidableObject> eng::d3::ContactsHandler::nextToVerticalSurfaces(const CollidableObject &object, bool previous_frame) const {
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
        return contact.axis.x != 0;
    })) {
        auto other_object = (contact.objectA == object) ? contact.objectB : contact.objectA;
        vertical_surfaces.push_back(other_object);
    }
    return vertical_surfaces;
}

[[nodiscard]] std::vector<eng::d3::Contact> eng::d3::ContactsHandler::getContacts() const{
    return contacts_vector;
}

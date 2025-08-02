//
// Created by Agamjeet Singh on 27/07/25.
//

#ifndef CONTACTSHANDLER_H
#define CONTACTSHANDLER_H
#include <unordered_map>
#include "../entity/CollidableObject.h"
#include "../events/Contact.h"
class Player;
enum class Facing;

struct ContactKeyHash {
    std::size_t operator()(const std::pair<std::reference_wrapper<CollidableObject>, std::reference_wrapper<CollidableObject>>& key) const {
        return std::hash<void*>{}(&key.first.get()) ^ std::hash<void*>{}(&key.second.get());
    }
};

struct ContactKeyEqual {
    bool operator()(
        const std::pair<std::reference_wrapper<CollidableObject>, std::reference_wrapper<CollidableObject> > &lhs,
                   const std::pair<std::reference_wrapper<CollidableObject>, std::reference_wrapper<CollidableObject>>& rhs) const {
        return &lhs.first.get() == &rhs.first.get() && &lhs.second.get() == &rhs.second.get();
    }
};

class ContactsHandler {
public:
    static ContactsHandler& getInstance() {
        static ContactsHandler instance;
        return instance;
    }

    void addContact(Contact contact) {
        auto key = std::make_pair(std::ref(contact.objectA), std::ref(contact.objectB));
        contacts.erase(key);
        contacts.emplace(key, contact);
        if (contact.objectA != contact.objectB) {
            contacts.erase({contact.objectB, contact.objectA});
        }
    }

    void removeContact(Contact contact) {
        contacts.erase({std::ref(contact.objectA), std::ref(contact.objectB)});
        contacts.erase({std::ref(contact.objectB), std::ref(contact.objectA)});
    }

    void reset() {
        contacts.clear();
    }

    [[nodiscard]] std::optional<Contact> areTouching(CollidableObject& objectA, CollidableObject& objectB) const {
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

    [[nodiscard]] std::vector<Contact> allContacts(const CollidableObject& object) const {

        std::vector<Contact> object_contacts;
        for (const auto& keyValue: contacts) {
            const auto [fst, snd] = keyValue.first;
            if (fst.get() == object || snd.get() == object) {
                object_contacts.push_back(keyValue.second);
            }
        }
        return object_contacts;
    }

    [[nodiscard]] bool onLand(const CollidableObject& object) const {
        auto all_contacts = allContacts(object);
        for (const auto& contact: all_contacts) {
            if (contact.axis == ContactAxis::Y) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] std::vector<CollidableObject> nextToVerticalSurfaces(const CollidableObject& object) const {
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

    [[nodiscard]] std::vector<Contact> restingOnSurfaces(const CollidableObject& object) const {
        std::vector<Contact> horizontal_contacts;
        auto all_contacts = allContacts(object);
        for (const auto& contact: all_contacts) {
            if (contact.axis == ContactAxis::Y) {
                horizontal_contacts.push_back(contact);
            }
        }
        return horizontal_contacts;
    }

    [[nodiscard]] const std::unordered_map<std::pair<std::reference_wrapper<CollidableObject>, std::reference_wrapper<CollidableObject>>, Contact, ContactKeyHash, ContactKeyEqual>& getContacts() const {
        return contacts;
    }

private:
    std::unordered_map<std::pair<std::reference_wrapper<CollidableObject>, std::reference_wrapper<CollidableObject>>, Contact, ContactKeyHash, ContactKeyEqual> contacts;
};



#endif //CONTACTSHANDLER_H

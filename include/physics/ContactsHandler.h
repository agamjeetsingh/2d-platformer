//
// Created by Agamjeet Singh on 27/07/25.
//

#ifndef CONTACTSHANDLER_H
#define CONTACTSHANDLER_H
#include <unordered_map>
#include "../entity/CollidableObject.h"
#include "../events/Contact.h"
#include "CollisionsHandler.h"

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
    static ContactsHandler& getInstance();

    /**
     * @brief Adds the contact into the contacts hashmap
     * @warning If the pair of objects in the contact map previously exist, in the same order, then it doesn't add the new contact
     * @param contact The contact to be added.
     */
    void addContact(Contact contact);

    void removeContact(Contact contact);

    void reset();

    [[nodiscard]] std::optional<Contact> areTouching(CollidableObject& objectA, CollidableObject& objectB) const;

    [[nodiscard]] std::vector<Contact> allContacts(const CollidableObject& object) const;

    [[nodiscard]] bool onLand(const CollidableObject& object) const;

    [[nodiscard]] std::vector<CollidableObject> nextToVerticalSurfaces(const CollidableObject& object) const;

    [[nodiscard]] std::vector<Contact> restingOnSurfaces(const CollidableObject& object) const;

    [[nodiscard]] const std::unordered_map<std::pair<std::reference_wrapper<CollidableObject>, std::reference_wrapper<CollidableObject>>, Contact, ContactKeyHash, ContactKeyEqual>& getContacts() const;

private:
    std::unordered_map<std::pair<std::reference_wrapper<CollidableObject>, std::reference_wrapper<CollidableObject>>, Contact, ContactKeyHash, ContactKeyEqual> contacts;
};



#endif //CONTACTSHANDLER_H

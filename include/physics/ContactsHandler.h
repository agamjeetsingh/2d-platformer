//
// Created by Agamjeet Singh on 27/07/25.
//

#ifndef CONTACTSHANDLER_H
#define CONTACTSHANDLER_H
#include <unordered_map>
#include "../entity/CollidableObject.h"
#include "../events/Contact.h"
#include "CollisionsHandler.h"
#include <ranges>

class Player;
enum class Facing;

/**
 * Hash function for \code std::reference_wrapper<const Collider>\endcode
 */
struct CollidableObjectConstRefHash {
    using is_transparent = void;
    std::size_t operator()(const std::reference_wrapper<CollidableObject>& ref) const noexcept {
        return std::hash<const CollidableObject*>{}(&ref.get());
    }

    std::size_t operator()(const CollidableObject& obj) const noexcept {
        return std::hash<const CollidableObject*>{}(&obj);
    }
};

/**
 * Equality function for \code std::reference_wrapper<const Collider>\endcode
 */
struct CollidableObjectConstRefEqual {
    using is_transparent = void;
    bool operator()(const std::reference_wrapper<CollidableObject>& lhs, const std::reference_wrapper<CollidableObject>& rhs) const noexcept {
        return &lhs.get() == &rhs.get();
    }
    bool operator()(const std::reference_wrapper<CollidableObject>& lhs, const CollidableObject& rhs) const noexcept {
        return &lhs.get() == &rhs;
    }
    bool operator()(const CollidableObject& lhs, const std::reference_wrapper<CollidableObject>& rhs) const noexcept {
        return &lhs == &rhs.get();
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

    void newFrame();

    [[nodiscard]] std::vector<Contact> allContacts(const CollidableObject& object) const;

    [[nodiscard]] bool onLand(const CollidableObject& object, bool previous_frame = false) const;

    [[nodiscard]] std::vector<CollidableObject> nextToVerticalSurfaces(const CollidableObject& object, bool previous_frame = false) const;

    [[nodiscard]] std::vector<Contact> restingOnSurfaces(const CollidableObject& object, bool previous_frame = false) const;

    [[nodiscard]] std::vector<Contact> getContacts() const;

    void emitPlayerLeftGroundEvent() const;

private:
    std::unordered_map<std::reference_wrapper<CollidableObject>, std::vector<Contact>, CollidableObjectConstRefHash, CollidableObjectConstRefEqual> contacts;
    std::unordered_map<std::reference_wrapper<CollidableObject>, std::vector<Contact>, CollidableObjectConstRefHash, CollidableObjectConstRefEqual> previous_frame_contacts;

    std::vector<Contact> contacts_vector;
};



#endif //CONTACTSHANDLER_H

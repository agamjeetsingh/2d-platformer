//
// Created by Agamjeet Singh on 28/07/25.
//

#ifndef ISLAND_H
#define ISLAND_H

#include <vector>
#include <functional>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class CollidableObject;

class Island : public std::enable_shared_from_this<Island> {
public:
    explicit Island(CollidableObject& leader);

    [[nodiscard]] const CollidableObject& getLeader() const;

    [[nodiscard]] std::vector<std::reference_wrapper<CollidableObject>> getNonLeaders() const;

    [[nodiscard]] const std::vector<std::reference_wrapper<CollidableObject>>& getMembers() const;

    void addMember(CollidableObject& object);

    bool removeMember(CollidableObject& object);

    [[nodiscard]] sf::Vector2f getLeaderVelocity() const;

    [[nodiscard]] sf::Vector2f getLeaderAcceleration() const;

    void setLeader(CollidableObject& new_leader);

    [[nodiscard]] bool isLeader(const CollidableObject& object) const {
        return &leader.get() == &object;
    }

protected:
    std::reference_wrapper<CollidableObject> leader;

private:
    std::vector<std::reference_wrapper<CollidableObject>> objects;
};


#endif //ISLAND_H

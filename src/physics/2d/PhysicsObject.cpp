//
// Created by Agamjeet Singh on 13/07/25.
//

#include "../../../include/physics/2d/PhysicsObject.h"

PhysicsObject::PhysicsObject(sf::Sprite sprite, sf::Vector2f position = {0, 0}) :
    sprite(std::move(sprite)),
    position(position) {
    this->sprite.setPosition(position);
}

const sf::Vector2f &PhysicsObject::getPositionRef() const {
    return position;
}

const sf::Sprite* PhysicsObject::getSprite() const {
    return &sprite;
}

sf::Vector2f PhysicsObject::getPosition() const {
    return position;
}

void PhysicsObject::setPosition(sf::Vector2f position) {
    this->position = position;
    sprite.setPosition(this->position);
}

void PhysicsObject::addPosition(sf::Vector2f position) {
    this->position += position;
    sprite.setPosition(this->position);
}

sf::Vector2f PhysicsObject::getTotalVelocity() const {
    return base_velocity + friction_velocity;
}

void PhysicsObject::printVelocity(const std::string &name) const {
    std::cout << name << " base_velocity: x = " << base_velocity.x << ", y = " << base_velocity.y << std::endl;
    std::cout << name << " friction_velocity: x = " << friction_velocity.x << ", y = " << friction_velocity.y << std::endl;
}

void PhysicsObject::printVelocity() const {
    printVelocity(getAddressAsString());
}

std::string PhysicsObject::getAddressAsString() const {
    std::ostringstream oss;
    oss << this;
    return oss.str();
}

void PhysicsObject::squeeze(sf::Vector2f target_squeeze, float time_to_squeeze, float time_to_unsqueeze) {
    auto discard1 = Scheduler::getInstance().schedule([total_time = 0.f, time_to_squeeze, target_squeeze, this, time_to_unsqueeze](const std::shared_ptr<ScheduledEvent>& event, float dt) mutable {
        total_time += dt;
        float t = std::min(total_time,time_to_squeeze);
        float alpha = t / time_to_squeeze;
        alpha = alpha * alpha * (3 - 2 * alpha);
        sf::Vector2f startScale = {1, 1};
        sf::Vector2f targetScale = target_squeeze;
        sf::Vector2f current = startScale * (1 - alpha) + targetScale * alpha;
        current.x *= sprite.getScale().x > 0 ? 1 : -1;
        current.y *= sprite.getScale().y > 0 ? 1 : -1;
        sprite.setScale(current);

        if (t == time_to_squeeze) {
            event->cancel();
            // Schedule next one
            auto discard2 = Scheduler::getInstance().schedule([total_time = 0.f, target_squeeze, time_to_unsqueeze, this](const std::shared_ptr<ScheduledEvent>& event, float dt) mutable {
                total_time += dt;
                float t = std::min(total_time,time_to_unsqueeze);
                float alpha = t / time_to_unsqueeze;
                alpha = alpha * alpha * (3 - 2 * alpha);
                sf::Vector2f startScale = target_squeeze;;
                sf::Vector2f targetScale = {1, 1};
                sf::Vector2f current = startScale * (1 - alpha) + targetScale * alpha;
                current.x *= sprite.getScale().x > 0 ? 1 : -1;
                current.y *= sprite.getScale().y > 0 ? 1 : -1;
                sprite.setScale(current);

                if (t == time_to_unsqueeze) {
                    event->cancel();
                }
            }, 0, true, 0);
        }
    }, 0, true, 0);
}



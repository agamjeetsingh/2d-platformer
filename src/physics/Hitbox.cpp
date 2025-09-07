//
// Created by Agamjeet Singh on 17/07/25.
//

#include "physics/Hitbox.h"

Hitbox::Hitbox(std::vector<sf::FloatRect> original_hitbox, const sf::Vector2f& position):
    original_hitbox(std::move(original_hitbox)),
    position(position) {}

[[nodiscard]] std::vector<sf::FloatRect> Hitbox::getRects() const {
    auto copied_hitbox = original_hitbox;
    for (auto& box: copied_hitbox) {
        box.position += position;
    }
    return copied_hitbox;
}

std::vector<sf::FloatRect> Hitbox::getUnshiftedRects() const {
    return original_hitbox;
}


void Hitbox::setRects(const std::vector<sf::FloatRect>& rects) {
    original_hitbox = rects;
}

sf::FloatRect Hitbox::getBounds() const {
    const auto first_hitbox = *begin();
    float leftmost_coord = first_hitbox.position.x;
    float rightmost_coord = first_hitbox.position.x + first_hitbox.size.x;
    float bottommost_coord = first_hitbox.position.y + first_hitbox.size.y;
    float topmost_coord = first_hitbox.position.y;

    for (const auto& box: *this) {
        leftmost_coord = std::min(leftmost_coord, box.position.x);
        rightmost_coord = std::max(rightmost_coord, box.position.x + box.size.x);
        bottommost_coord = std::max(bottommost_coord, box.position.y + box.size.y);
        topmost_coord = std::min(topmost_coord, box.position.y);
    }

    return {{leftmost_coord, topmost_coord}, {rightmost_coord - leftmost_coord, bottommost_coord - topmost_coord}};
}

Hitbox::Iterator::Iterator(const std::vector<sf::FloatRect>& original_hitbox, std::vector<sf::FloatRect>::size_type current_index, sf::Vector2f position):
        original_hitbox(original_hitbox),
        current_index(current_index),
        position(position) {}

sf::FloatRect Hitbox::Iterator::operator*() const {
    sf::FloatRect shifted = original_hitbox[current_index];
    shifted.position += position;
    return shifted;
}

Hitbox::Iterator& Hitbox::Iterator::operator++() {
    current_index++;
    return *this;
}

bool Hitbox::Iterator::operator!=(const Iterator& other) const {
    return current_index != other.current_index || &original_hitbox != &other.original_hitbox;
}

[[nodiscard]] Hitbox::Iterator Hitbox::begin() const {
    return Iterator(original_hitbox, 0, position);
}

[[nodiscard]] Hitbox::Iterator Hitbox::end() const {
    return Iterator(original_hitbox, original_hitbox.size(), position);
}

size_t Hitbox::getSize() const {
    return original_hitbox.size();
}


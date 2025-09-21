//
// Created by Agamjeet Singh on 21/09/25.
//

#include "Hitbox.h"

eng::d3::Hitbox::Hitbox(std::vector<Box> original_hitbox, const Vector3f& position):
    original_hitbox(std::move(original_hitbox)),
    position(position) {}

[[nodiscard]] std::vector<eng::d3::Box> eng::d3::Hitbox::getBoxes() const {
    auto copied_hitbox = original_hitbox;
    for (auto& box: copied_hitbox) {
        box.position += position;
    }
    return copied_hitbox;
}

std::vector<eng::d3::Box> eng::d3::Hitbox::getUnshiftedBoxes() const {
    return original_hitbox;
}


void eng::d3::Hitbox::setBoxes(const std::vector<Box>& boxes) {
    original_hitbox = boxes;
}

eng::d3::Box eng::d3::Hitbox::getBounds() const {
    const auto first_hitbox = *begin();
    float min_x = first_hitbox.position.x;
    float max_x = first_hitbox.position.x + first_hitbox.size.x;
    float min_y = first_hitbox.position.y;
    float max_y = first_hitbox.position.y + first_hitbox.size.y;
    float min_z = first_hitbox.position.z;
    float max_z = first_hitbox.position.z + first_hitbox.size.z;

    for (const auto& box : *this) {
        min_x = std::min(min_x, box.position.x);
        max_x = std::max(max_x, box.position.x + box.size.x);

        min_y = std::min(min_y, box.position.y);
        max_y = std::max(max_y, box.position.y + box.size.y);

        min_z = std::min(min_z, box.position.z);
        max_z = std::max(max_z, box.position.z + box.size.z);
    }

    return {{min_x, min_y, min_z}, {max_x - min_x, max_y - min_y, max_z - min_z}};
}

eng::d3::Hitbox::Iterator::Iterator(const std::vector<Box>& original_hitbox, std::vector<Box>::size_type current_index, Vector3f position):
        original_hitbox(original_hitbox),
        current_index(current_index),
        position(position) {}

eng::d3::Box eng::d3::Hitbox::Iterator::operator*() const {
    Box shifted = original_hitbox[current_index];
    shifted.position += position;
    return shifted;
}

eng::d3::Hitbox::Iterator& eng::d3::Hitbox::Iterator::operator++() {
    current_index++;
    return *this;
}

bool eng::d3::Hitbox::Iterator::operator!=(const Iterator& other) const {
    return current_index != other.current_index || &original_hitbox != &other.original_hitbox;
}

[[nodiscard]] eng::d3::Hitbox::Iterator eng::d3::Hitbox::begin() const {
    return Iterator(original_hitbox, 0, position);
}

[[nodiscard]] eng::d3::Hitbox::Iterator eng::d3::Hitbox::end() const {
    return Iterator(original_hitbox, original_hitbox.size(), position);
}

size_t eng::d3::Hitbox::getSize() const {
    return original_hitbox.size();
}

//
// Created by Agamjeet Singh on 21/09/25.
//

#include "Box.h"

constexpr bool eng::d3::Box::contains(Vector3f point) const {
    return (position.x <= point.x && position.x + size.x >= point.x) &&
            (position.y <= point.y && position.y + size.y >= point.y) &&
            (position.z <= point.z && position.z + size.z >= point.z);
}

constexpr std::optional<eng::d3::Box> eng::d3::Box::findIntersection(const Box &other) const {
    const float xMin = std::max(position.x, other.position.x);
    const float yMin = std::max(position.y, other.position.y);
    const float zMin = std::max(position.z, other.position.z);

    const float xMax = std::min(position.x + size.x, other.position.x + other.size.x);
    const float yMax = std::min(position.y + size.y, other.position.y + other.size.y);
    const float zMax = std::min(position.z + size.z, other.position.z + other.size.z);

    if (xMin < xMax && yMin < yMax && zMin < zMax) {
        return Box({xMin, yMin, zMin},{xMax - xMin, yMax - yMin, zMax - zMin});
    }
    return std::nullopt;
}



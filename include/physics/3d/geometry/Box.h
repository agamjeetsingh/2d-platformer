//
// Created by Agamjeet Singh on 21/09/25.
//

#ifndef BOX_H
#define BOX_H
#include <algorithm>
#include <optional>

#include "Vector3f.h"


namespace eng::d3 {
    class Box {
    public:
        Box(const Vector3f position, const Vector3f size): position(position), size(size) {}
        Vector3f position;
        Vector3f size;

        [[nodiscard]] constexpr bool contains(Vector3f point) const;

        [[nodiscard]] constexpr std::optional<Box> findIntersection(const Box& other) const;

        [[nodiscard]] bool operator==(const Box& other) const { return position == other.position && size == other.size; }
    };
}



#endif //BOX_H

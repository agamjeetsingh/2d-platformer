//
// Created by Agamjeet Singh on 21/09/25.
//

#include "../../../include/physics/3d/SpacialHashMap.h"
#include "../../../include/physics/3d/CollidableObject.h"
#include "../../../include/physics/3d/CollisionsHandler.h"

eng::d3::SpacialHashMap::SpacialHashMap(CollisionsHandler* collisions_handler, size_t size) : num_buckets(size), collisions_handler(collisions_handler) {
    buckets = std::vector<std::vector<CollidableObject*>>{4 * size / 3};
}

void eng::d3::SpacialHashMap::clear() {
    for (auto &bucket : buckets) {
        bucket.clear();
    }
    num_objects = 0;
}


void eng::d3::SpacialHashMap::addObject(CollidableObject *ptr) {
    for (size_t hash: getHashes(ptr)) {
        buckets[hash % num_buckets].push_back(ptr);
    }
    num_objects++;
}

std::vector<std::pair<eng::d3::CollidableObject *, eng::d3::CollidableObject *> > eng::d3::SpacialHashMap::getPairs() const {
    std::vector<std::pair<CollidableObject*, CollidableObject*>> pairs;
    for (const auto& bucket: buckets) {
        for (auto ptr1: bucket) {
            for (auto ptr2: bucket) {
                if (ptr1 <= ptr2) {
                    continue;
                }
                pairs.emplace_back(ptr1, ptr2);
            }
        }
    }
    return pairs;
}

std::vector<size_t> eng::d3::SpacialHashMap::getHashes(const CollidableObject *ptr) const {
    const float cellSize = collisions_handler->getCellSize();
    const auto bounds = ptr->getHitbox().getBounds();

    const int cellX_min = static_cast<int>(std::floor(bounds.position.x / cellSize));
    const int cellY_min = static_cast<int>(std::floor(bounds.position.y / cellSize));
    const int cellZ_min = static_cast<int>(std::floor(bounds.position.z / cellSize));
    const int cellX_max = static_cast<int>(std::floor((bounds.position.x + bounds.size.x) / cellSize));
    const int cellY_max = static_cast<int>(std::floor((bounds.position.y + bounds.size.y) / cellSize));
    const int cellZ_max = static_cast<int>(std::floor((bounds.position.z + bounds.size.z) / cellSize));

    std::vector<size_t> hashes;
    hashes.reserve((cellX_max - cellX_min + 1) * (cellY_max - cellY_min + 1) * (cellZ_max - cellZ_min + 1));
    for (int cellX = cellX_min; cellX <= cellX_max; ++cellX) {
        for (int cellY = cellY_min; cellY <= cellY_max; ++cellY) {
            for (int cellZ = cellZ_min; cellZ <= cellZ_max; ++cellZ) {
                const size_t hash = std::hash<int>{}(cellX) ^ (std::hash<int>{}(cellY) << 1) ^ (std::hash<int>{}(cellZ) << 2);
                hashes.push_back(hash);
            }
        }
    }

    return hashes;
}

//
// Created by Agamjeet Singh on 09/08/25.
//

#include "../../include/physics/SpacialHashMap.h"
#include "../../include/physics/CollisionsHandler.h"

SpacialHashMap::SpacialHashMap(size_t size) : num_buckets(size) {
    buckets = std::vector<std::vector<CollidableObject*>>{4 * size / 3};
}

void SpacialHashMap::clear() {
    for (auto &bucket : buckets) {
        bucket.clear();
    }
}


void SpacialHashMap::addObject(CollidableObject *ptr) {
    for (size_t hash: getHashes(ptr)) {
        buckets[hash % num_buckets].push_back(ptr);
    }
}

std::vector<std::pair<CollidableObject *, CollidableObject *> > SpacialHashMap::getPairs() const {
    std::vector<std::pair<CollidableObject*, CollidableObject*>> pairs;
    for (const auto& bucket: buckets) {
        for (auto ptr1: bucket) {
            for (auto ptr2: bucket) {
                if (&ptr1 <= &ptr2) {
                    continue;
                }
                pairs.emplace_back(ptr1, ptr2);
            }
        }
    }
    return std::move(pairs);
}

std::vector<size_t> SpacialHashMap::getHashes(const CollidableObject *ptr) {
    const float cellSize = CollisionsHandler::getInstance().getCellSize();
    const auto bounds = ptr->getHitbox().getBounds();
    
    const int cellX_min = static_cast<int>(std::floor(bounds.position.x / cellSize));
    const int cellY_min = static_cast<int>(std::floor(bounds.position.y / cellSize));
    const int cellX_max = static_cast<int>(std::floor((bounds.position.x + bounds.size.x) / cellSize));
    const int cellY_max = static_cast<int>(std::floor((bounds.position.y + bounds.size.y) / cellSize));
    
    std::vector<size_t> hashes;
    hashes.reserve((cellX_max - cellX_min + 1) * (cellY_max - cellY_min + 1));
    for (int cellX = cellX_min; cellX <= cellX_max; ++cellX) {
        for (int cellY = cellY_min; cellY <= cellY_max; ++cellY) {
            const size_t hash = std::hash<int>{}(cellX) ^ (std::hash<int>{}(cellY) << 1);
            hashes.push_back(hash);
        }
    }

    return hashes;
}




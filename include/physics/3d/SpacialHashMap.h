//
// Created by Agamjeet Singh on 21/09/25.
//

#ifndef SPACIALHASHMAP_H
#define SPACIALHASHMAP_H
#include <vector>

namespace eng::d3 {
    class CollisionsHandler;
    class CollidableObject;
    class SpacialHashMap {
    public:
        explicit SpacialHashMap(CollisionsHandler* collisions_handler, size_t size = DEFAULT_BUCKETS);

        [[nodiscard]] size_t getSize() const {
            return num_buckets;
        }

        [[nodiscard]] size_t getNumObjects() const {
            return num_objects;
        }

        void clear();

        void addObject(CollidableObject* ptr);

        [[nodiscard]] std::vector<std::pair<CollidableObject*, CollidableObject*>> getPairs() const;

    private:
        std::vector<std::vector<CollidableObject*>> buckets;

        size_t num_buckets;

        size_t num_objects = 0;

        CollisionsHandler* collisions_handler;

        std::vector<size_t> getHashes(const CollidableObject* ptr) const;

        static constexpr size_t DEFAULT_BUCKETS = 16;
    };
}



#endif //SPACIALHASHMAP_H

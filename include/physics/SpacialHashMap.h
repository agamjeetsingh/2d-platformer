//
// Created by Agamjeet Singh on 09/08/25.
//

#ifndef SPACIALHASHMAP_H
#define SPACIALHASHMAP_H
#include <vector>

class CollidableObject;

class SpacialHashMap {
public:
    explicit SpacialHashMap(size_t size = DEFAULT_BUCKETS);

    [[nodiscard]] size_t getSize() const {
        return num_buckets;
    }
    void clear();

    void addObject(CollidableObject* ptr);

    [[nodiscard]] std::vector<std::pair<CollidableObject*, CollidableObject*>> getPairs() const;

private:
    std::vector<std::vector<CollidableObject*>> buckets;

    size_t num_buckets;

    static std::vector<size_t> getHashes(const CollidableObject* ptr);

    static constexpr size_t DEFAULT_BUCKETS = 16;
};



#endif //SPACIALHASHMAP_H

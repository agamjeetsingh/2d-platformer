//
// Created by Agamjeet Singh on 09/08/25.
//

#ifndef SPACIALHASHMAP_H
#define SPACIALHASHMAP_H
#include <vector>

class CollisionsHandler;
class CollidableObject;

class HashIterator {
public:
    HashIterator(int x, int y, int minX, int maxX, int minY, int maxY)
        : x(x), y(y), minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}

    size_t operator*() const {
        return std::hash<int>{}(x) ^ (std::hash<int>{}(y) << 1);
    }

    HashIterator& operator++() {
        if (++y > maxY) {
            y = minY;
            ++x;
        }
        return *this;
    }

    bool operator!=(const HashIterator& other) const {
        return x != other.x || y != other.y;
    }

private:
    int x, y;
    int minX, maxX, minY, maxY;
};

struct HashRange {
    HashIterator begin_, end_;
    [[nodiscard]] HashIterator begin() const { return begin_; }
    [[nodiscard]] HashIterator end() const { return end_; }
};


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

    HashRange getHashesFaster(const CollidableObject* ptr) const;

    static constexpr size_t DEFAULT_BUCKETS = 16;
};



#endif //SPACIALHASHMAP_H

//
// Created by Agamjeet Singh on 11/09/25.
//

#ifndef RANDOM_H
#define RANDOM_H
#include <random>
#include <SFML/Graphics.hpp>

class Random {
public:
    struct IntRange {
        IntRange(int min, int max): min(min), max(max) {}
        const int min;
        const int max;

        [[nodiscard]] int getRandom() const {
            return Random::getInstance().getInt(min, max);
        }
    };

    struct Vector2fRange {
        Vector2fRange(sf::Vector2f min, sf::Vector2f max): min(min), max(max) {}
        const sf::Vector2f min;
        const sf::Vector2f max;

        [[nodiscard]] sf::Vector2f getRandom() const {
            float x = Random::getInstance().getFloat(min.x, max.x);
            float y = Random::getInstance().getFloat(min.y, max.y);
            return {x, y};
        }
    };

    struct FloatRange {
        FloatRange(float min, float max): min(min), max(max) {}
        const float min;
        const float max;

        [[nodiscard]] float getRandom() const {
            return Random::getInstance().getFloat(min, max);
        }
    };

    int getInt(const int min, const int max) {
        std::uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }

    float getFloat(const float min, const float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(gen);
    }

    static Random& getInstance() {
        static Random instance;
        return instance;
    }

    std::random_device rd;
    std::mt19937 gen{rd()};
};



#endif //RANDOM_H

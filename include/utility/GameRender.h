//
// Created by Agamjeet Singh on 27/08/25.
//

#ifndef GAMERENDER_H
#define GAMERENDER_H
#include <iostream>
#include <memory>
#include <set>
#include <SFML/Graphics.hpp>

template <typename T>
concept Drawable = requires(T t) {
    { t.getSprite() } -> std::convertible_to<const sf::Drawable*>;
};

template <typename T>
concept DrawableLike = std::is_base_of_v<sf::Drawable, T>;

struct zComparator {
    bool operator()(const std::pair<std::function<std::optional<const sf::Drawable*>(float)>, float>& p1, const std::pair<std::function<std::optional<const sf::Drawable*>(float)>, float>& p2) const {
        return p1.second < p2.second;
    }
};

class GameRender {
public:
    template <Drawable T>
    void registerDrawable(std::shared_ptr<T> object, float z = 0) {
        drawables.insert(std::make_pair([weak = std::weak_ptr<T>(object)](float dt) -> std::optional<const sf::Drawable*> {
            if (auto shared = weak.lock()) {
                if constexpr (requires(T x) { x.updateSprite(dt); }) {
                    shared->updateSprite(dt);
                }
                return shared->getSprite();
            }
            return std::nullopt;
        }, z));
    }

    template <Drawable T>
    void draw(std::shared_ptr<T> object, float z = 0) {
        drawables.insert(std::make_pair([weak = std::weak_ptr<T>(object), count = 0](float dt) mutable -> std::optional<const sf::Drawable*> {
            if (auto shared = weak.lock(); (count == 0) && shared) {
                count++;
                return shared->getSprite();
            }
            return std::nullopt;
        }, z));
    }

    template <DrawableLike T>
    void drawSimpleDrawable(T drawable, float z = 0) {
        // No texture should be needed to be alive
        drawables.insert(std::make_pair([sprite=std::move(drawable), count = 0](float dt) mutable -> std::optional<const sf::Drawable*> {
            if (count == 0) {
                count++;
                return &sprite;
            }
            return std::nullopt;
        }, z));
    }

    void render(sf::RenderWindow& window, float dt);

    void shake(sf::Vector2f direction, float amplitude = 10, float frequency = 8, float damping = 2.f) {
        const float len = direction.length();
        assert(len > 0);
        direction /= len;
        shake_direction = direction;
        shake_amplitude = amplitude;
        shake_frequency = frequency;
        shake_damping = damping;
        shaking = true;
    }

    static GameRender& getInstance();

private:
    GameRender();

    sf::RenderTexture render_texture;
    std::multiset<std::pair<std::function<std::optional<const sf::Drawable*>(float)>, float>, zComparator> drawables;

    sf::Vector2f shake_direction = {1, 1};
    bool shaking = false;
    float shake_time = 0;
    float shake_amplitude = 0;
    float shake_frequency = 0;
    float shake_damping = 0;
};



#endif //GAMERENDER_H

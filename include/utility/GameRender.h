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
    bool operator()(const std::pair<std::function<std::optional<const sf::Drawable*>()>, float>& p1, const std::pair<std::function<std::optional<const sf::Drawable*>()>, float>& p2) const {
        return p1.second < p2.second;
    }
};

class GameRender {
public:
    template <Drawable T>
    void registerDrawable(std::shared_ptr<T> object, float z = 0) {
        drawables.insert(std::make_pair([weak = std::weak_ptr<T>(object)]() -> std::optional<const sf::Drawable*> {
            if (auto shared = weak.lock()) {
                return shared->getSprite();
            }
            return std::nullopt;
        }, z));
    }

    template <Drawable T>
    void draw(std::shared_ptr<T> object, float z = 0) {
        drawables.insert(std::make_pair([weak = std::weak_ptr<T>(object), count = 0]() mutable -> std::optional<const sf::Drawable*> {
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
        // render_texture.draw(sprite);

        drawables.insert(std::make_pair([sprite=std::move(drawable), count = 0]() mutable -> std::optional<const sf::Drawable*> {
            if (count == 0) {
                count++;
                return &sprite;
            }
            return std::nullopt;
        }, z));
    }

    void render(sf::RenderWindow& window);

    static GameRender& getInstance();

private:
    GameRender();

    sf::RenderTexture render_texture;
    std::multiset<std::pair<std::function<std::optional<const sf::Drawable*>()>, float>, zComparator> drawables;
};



#endif //GAMERENDER_H

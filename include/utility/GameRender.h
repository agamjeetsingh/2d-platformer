//
// Created by Agamjeet Singh on 27/08/25.
//

#ifndef GAMERENDER_H
#define GAMERENDER_H
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

template <typename T>
concept Drawable = requires(T t) {
    { t.getSprite() } -> std::convertible_to<sf::Sprite>;
};

class GameRender {
public:
    template <Drawable T>
    void registerDrawable(std::shared_ptr<T> object) {
        drawables.emplace_back([weak = std::weak_ptr<T>(object)]() -> std::optional<sf::Sprite> {
            if (auto shared = weak.lock()) {
                return shared->getSprite();
            }
            return std::nullopt;
        });
    }

    template <Drawable T>
    void draw(std::shared_ptr<T> object) {
        render_texture.draw(object->getSprite());
    }

    void render(sf::RenderWindow& window);

    static GameRender& getInstance();

private:
    GameRender();

    sf::RenderTexture render_texture;
    std::vector<std::function<std::optional<sf::Sprite>()>> drawables;
};



#endif //GAMERENDER_H

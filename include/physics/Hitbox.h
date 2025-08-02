//
// Created by Agamjeet Singh on 17/07/25.
//

#ifndef HITBOX_H
#define HITBOX_H
#include <unordered_set>
#include <vector>

#include "../../../../../../opt/homebrew/Cellar/sfml/3.0.1/include/SFML/Graphics/Rect.hpp"

/**
 * Handles shifting the hitbox with the position of the object.
 */
class Hitbox {
public:
    /**
     * @brief Constructs the Hitbox with the given original hitbox and position. The hitbox cannot be empty.
     * @param original_hitbox The original hitbox of the object.
     * @param position The position of the object that the hitbox would be shifted by.
     */
    Hitbox(std::vector<sf::FloatRect> original_hitbox, const sf::Vector2f& position);

    /**
     * @brief Gives the shifted hitbox.
     * @return the hitbox shifted by the position.
     */
    [[nodiscard]] std::vector<sf::FloatRect> getRects() const;

    [[nodiscard]] sf::FloatRect getBounds() const;

    /**
     * @brief Iterator to iterate over the shifted hitboxes
     */
    class Iterator {
    public:
        explicit Iterator(const std::vector<sf::FloatRect>& original_hitbox, std::vector<sf::FloatRect>::size_type current_index, sf::Vector2f position);

        sf::FloatRect operator*() const;

        Iterator& operator++();

        bool operator!=(const Iterator& other) const;

    private:
        const std::vector<sf::FloatRect>& original_hitbox;
        std::vector<sf::FloatRect>::size_type current_index;
        sf::Vector2f position;
    };

    /**
     * @brief Returns an iterator to the beginning of the hitbox rectangles.
     * @return Iterator pointing to the first rectangle (position-shifted).
     */
    [[nodiscard]] Iterator begin() const;
    
    /**
     * @brief Returns an iterator to the end of the hitbox rectangles.
     * @return Iterator pointing one past the last rectangle.
     */
    [[nodiscard]] Iterator end() const;

private:
    std::vector<sf::FloatRect> original_hitbox;
    const sf::Vector2f& position;
};

#endif //HITBOX_H

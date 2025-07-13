//
// Created by Agamjeet Singh on 12/07/25.
//

#pragma once

#include <set>
#include "../../../../../../opt/homebrew/Cellar/sfml/3.0.1/include/SFML/Window/Keyboard.hpp"

/**
 * \brief Manages all inputs. Can be used to check whether a key is currently being pressed. A better alternative to
 * SFML's \code sf::Keyboard::isKeyPressed\endcode since that requires macOS input monitoring permissions. Needs to be
 * kept updated through \code main.cpp\endcode.
 */
class InputManager {
public:

    /**
      * @brief Gets the singleton instance of InputManager
      * @return Reference to the single InputManager instance
      */
    static InputManager& getInstance() {
        static InputManager instance;
        return instance;
    }

    /**
     * @brief Marks a key as pressed
     * @param key The keyboard key that was pressed
     */
    void keyPressed(const sf::Keyboard::Key key) { pressedKeys.insert(key); }

    /**
     * @brief Marks a key as released
     * @param key The keyboard key that was released
     */
    void keyReleased(const sf::Keyboard::Key key) { pressedKeys.erase(key); }

    /**
     * @brief Checks if a key is currently pressed
     * @param key The keyboard key to check
     * @return true if and only if the key is currently pressed
     */
    [[nodiscard]] bool isPressed(const sf::Keyboard::Key key) const { return pressedKeys.contains(key); }

private:
    std::set<sf::Keyboard::Key> pressedKeys;
};

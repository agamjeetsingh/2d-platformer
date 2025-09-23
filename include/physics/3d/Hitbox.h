//
// Created by Agamjeet Singh on 21/09/25.
//

#ifndef HITBOX_H
#define HITBOX_H
#include <vector>

#include "geometry/Box.h"

namespace eng::d3 {
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
        Hitbox(std::vector<Box> original_hitbox, const Vector3f& position);

        /**
         * @brief Gives the shifted hitbox.
         * @return the hitbox shifted by the position.
         */
        [[nodiscard]] std::vector<Box> getBoxes() const;

        [[nodiscard]] std::vector<Box> getUnshiftedBoxes() const;

        void setBoxes(const std::vector<Box> &boxes);

        [[nodiscard]] Box getBounds() const;

        [[nodiscard]] size_t getSize() const;

        /**
         * @brief Iterator to iterate over the shifted hitboxes
         */
        class Iterator {
        public:
            explicit Iterator(const std::vector<Box>& original_hitbox, std::vector<Box>::size_type current_index, Vector3f position);

            Box operator*() const;

            Iterator& operator++();

            bool operator!=(const Iterator& other) const;

        private:
            const std::vector<Box>& original_hitbox;
            std::vector<Box>::size_type current_index;
            Vector3f position;
        };

        /**
         * @brief Returns an iterator to the beginning of the hitbox boxes.
         * @return Iterator pointing to the first box (position-shifted).
         */
        [[nodiscard]] Iterator begin() const;

        /**
         * @brief Returns an iterator to the end of the hitbox boxes.
         * @return Iterator pointing one past the last box.
         */
        [[nodiscard]] Iterator end() const;

    private:
        std::vector<Box> original_hitbox;
        const Vector3f& position;
    };
}




#endif //HITBOX_H

//
// Created by Agamjeet Singh on 05/09/25.
//

#ifndef TOUCHSWITCH_H
#define TOUCHSWITCH_H
#include "TouchSwitchState.h"
#include "entity/CollidableObject.h"
#include "entity/sprites/TexturesHolder.h"
#include "events/Collision.h"
#include "utility/EmptyTextures.h"
#include "utility/GameRender.h"


class TouchSwitch final : public CollidableObject {
public:
    explicit TouchSwitch(sf::Vector2f position = {0, 0});

    void updateSprite(float deltaTime);

    void registerOther(TouchSwitch& other);

    bool isActivated() const;

    void setActive(bool active);

    bool canCollideWith(const CollidableObject& other, Collision collision) const override;

    bool areAllActivated() const;

    [[nodiscard]] static std::vector<std::shared_ptr<TouchSwitch>> makeTouchSwitches(const std::vector<sf::Vector2f>& positions);

private:
    bool activated = false;
    std::vector<std::reference_wrapper<TouchSwitch>> others;

    TouchSwitchState state = TouchSwitchState::NOT_ACTIVE;
    float time_in_state = 0;
    size_t curr_sprite_index = 0;

    std::unordered_map<TouchSwitchState, TexturesHolder> textures;
    sf::Texture container_texture;
    sf::RenderTexture combined_render_texture;
};



#endif //TOUCHSWITCH_H

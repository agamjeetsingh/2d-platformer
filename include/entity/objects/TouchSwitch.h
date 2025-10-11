//
// Created by Agamjeet Singh on 05/09/25.
//

#ifndef TOUCHSWITCH_H
#define TOUCHSWITCH_H
#include "TouchSwitchState.h"
#include "entity/sprites/TexturesHolder.h"
#include "../../physics/2d/Collision.h"
#include "events/Listener.h"
#include "physics/2d/CollidableObject.h"
#include "utility/EmptyTextures.h"
#include "utility/GameRender.h"
#include "utility/SoundEffect.h"
#include "utility/SoundManager.h"
#include <ranges>
#include <algorithm>

class TouchSwitch final : public CollidableObject {
public:
    explicit TouchSwitch(SoundManager<SoundEffect>& sound_manager, sf::Vector2f position = {0, 0});

    void updateSprite(float deltaTime);

    void registerOther(TouchSwitch& other);

    bool isActivated() const;

    void setActive(bool active);

    bool canCollideWith(const CollidableObject& other, Collision collision) const override;

    bool areAllActivated() const;

    [[nodiscard]] static std::vector<std::shared_ptr<TouchSwitch>> makeTouchSwitches(const std::vector<sf::Vector2f>& positions, SoundManager<SoundEffect>& sound_manager);

private:
    bool activated = false;
    std::vector<std::reference_wrapper<TouchSwitch>> others;

    TouchSwitchState state = TouchSwitchState::NOT_ACTIVE;

    std::unordered_map<TouchSwitchState, TexturesHolder> textures;
    std::unordered_map<TouchSwitchState, sf::Texture> container_textures;
    sf::RenderTexture combined_render_texture;

    Listener collision_listener;
    TouchSwitch* leader;
    SoundManager<SoundEffect>& sound_manager;

    void updateState();
};



#endif //TOUCHSWITCH_H

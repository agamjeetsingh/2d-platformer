//
// Created by Agamjeet Singh on 05/09/25.
//

#include "../../../include/entity/objects/TouchSwitch.h"

#include "utility/SoundManager.h"

TouchSwitch::TouchSwitch(sf::Vector2f position):
CollidableObject({{{2, 2}, {16, 16}}}, sf::Sprite{{EmptyTextures::getInstance().getEmpty({20, 20})}}, position),
collision_listener(Listener::make_listener<Collision>([this](const Collision& collision) {
    if (&collision.objectA != this && &collision.objectB != this) return;
    CollidableObject& other = &collision.objectA == this ? collision.objectB : collision.objectA;
    if (!other.isPlayer()) return;
    setActive(true);
})),
leader(this) {
    textures.emplace(TouchSwitchState::NOT_ACTIVE, TexturesHolder(6, "../assets/touchSwitch/inactive/inactive_icon"));
    textures.emplace(TouchSwitchState::ACTIVE, TexturesHolder(6, "../assets/touchSwitch/active/active_icon"));
    textures.emplace(TouchSwitchState::ALL_ACTIVE, TexturesHolder(6, "../assets/touchSwitch/all_active/all_active_icon"));
    container_textures.emplace(TouchSwitchState::NOT_ACTIVE, "../assets/touchSwitch/inactive/inactive_container.png");
    container_textures.emplace(TouchSwitchState::ACTIVE, "../assets/touchSwitch/active/active_container.png");
    container_textures.emplace(TouchSwitchState::ALL_ACTIVE, "../assets/touchSwitch/all_active/all_active_container.png");
    assert(combined_render_texture.resize({sf::VideoMode::getDesktopMode().size}));
}

void TouchSwitch::updateSprite(float deltaTime) {
    updateState();

    if (!textures.contains(state)) {
        return;
    }

    const TexturesHolder& touch_switch_textures = textures.at(state);

    time_in_state += deltaTime;

    while (touch_switch_textures.getIntervals()[curr_sprite_index] <= time_in_state) {
        if (curr_sprite_index == touch_switch_textures.getTextures().size()) {
            curr_sprite_index = 0;
        }
        sprite.setTexture(touch_switch_textures.getTextures()[curr_sprite_index]);
        time_in_state -= touch_switch_textures.getIntervals()[curr_sprite_index++];
    }
    combined_render_texture.clear(sf::Color::Transparent);
    sf::Sprite container_sprite = sf::Sprite{container_textures[state]};
    sf::Sprite front_sprite = sf::Sprite{touch_switch_textures.getTextures()[curr_sprite_index]};
    container_sprite.setPosition(getPosition());
    front_sprite.setPosition(getPosition());
    combined_render_texture.draw(container_sprite);
    combined_render_texture.draw(front_sprite);
    combined_render_texture.display();
    const sf::Sprite combined(combined_render_texture.getTexture());
    sprite = combined;
}

void TouchSwitch::registerOther(TouchSwitch &other) {
    others.push_back(std::ref(other));
    if (this < &other) {
        leader = &other;
    }
}

bool TouchSwitch::isActivated() const {
    return activated;
}

void TouchSwitch::setActive(bool active) {
    activated = active;
}

bool TouchSwitch::canCollideWith(const CollidableObject &other, Collision collision) const {
    return false;
}

bool TouchSwitch::areAllActivated() const {
    return activated && std::ranges::all_of(others, [](const std::reference_wrapper<TouchSwitch>& touch_switch) {
        return touch_switch.get().isActivated();
    });
}

std::vector<std::shared_ptr<TouchSwitch> > TouchSwitch::makeTouchSwitches(const std::vector<sf::Vector2f>& positions)  {
    std::vector<std::shared_ptr<TouchSwitch>> switches;

    switches.reserve(positions.size());
    for (auto& pos : positions) {
        switches.push_back(std::make_shared<TouchSwitch>(pos));
    }

    for (int i = 0; i < switches.size(); i++) {
        for (int j = 0; j < switches.size(); j++) {
            if (i != j) {
                switches[i]->registerOther(*switches[j]);
            }
        }
    }

    return switches;
}

void TouchSwitch::updateState() {
    TouchSwitchState prev_state = state;
    if (!activated) {
        state = TouchSwitchState::NOT_ACTIVE;
    } else if (!areAllActivated()) {
        state = TouchSwitchState::ACTIVE;
    } else {
        state = TouchSwitchState::ALL_ACTIVE;
    }
    if (prev_state == TouchSwitchState::NOT_ACTIVE && state == TouchSwitchState::ACTIVE) {
        SoundManager::getInstance().play(SoundEffect::TOUCH_SWITCH_ACTIVATE);
    }
    if (prev_state != TouchSwitchState::ALL_ACTIVE && state == TouchSwitchState::ALL_ACTIVE && this == leader) {
        SoundManager::getInstance().play(SoundEffect::TOUCH_SWITCH_ALL_ACTIVATE);
    }
}

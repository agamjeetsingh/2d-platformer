//
// Created by Agamjeet Singh on 05/09/25.
//

#include "../../../include/entity/objects/TouchSwitch.h"

TouchSwitch::TouchSwitch(sf::Vector2f position): CollidableObject({{{2, 2}, {16, 16}}}, sf::Sprite{{EmptyTextures::getInstance().getEmpty({20, 20})}}, position) {
    textures.emplace(TouchSwitchState::NOT_ACTIVE, TexturesHolder(6, "../assets/touchSwitch/icon"));
    assert(container_texture.loadFromFile("../assets/touchSwitch/container.png"));
    assert(combined_render_texture.resize({sf::VideoMode::getDesktopMode().size}));
}

void TouchSwitch::updateSprite(float deltaTime) {
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
    std::cout << combined_render_texture.getSize().x << combined_render_texture.getSize().y << std::endl;
    combined_render_texture.clear(sf::Color::Transparent);
    sf::Sprite container_sprite = sf::Sprite{container_texture};
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
}

bool TouchSwitch::isActivated() const {
    return activated;
}

void TouchSwitch::setActive(bool active) {
    activated = active;
    if (!activated) {
        state = TouchSwitchState::NOT_ACTIVE;
    } else if (!areAllActivated()) {
        state = TouchSwitchState::ACTIVE;
    } else {
        state = TouchSwitchState::ALL_ACTIVE;
    }
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







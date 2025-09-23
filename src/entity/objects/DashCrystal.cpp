//
// Created by Agamjeet Singh on 08/09/25.
//

#include "../../../include/entity/objects/DashCrystal.h"

#include "entity/PointParticles.h"

DashCrystal::DashCrystal(sf::Vector2f position, SoundManager& sound_manager): CollidableObject({{{3, 3}, {10, 10}}}, sf::Sprite{EmptyTextures::getInstance().getEmpty({16, 16})}, position),
                                                sound_manager(sound_manager),
                                                 collision_listener(Listener::make_listener<Collision>([this, &sound_manager](const Collision& collision) {
                                                     if (&collision.objectA != this && &collision.objectB != this) return;
                                                     CollidableObject& other = &collision.objectA == this ? collision.objectB : collision.objectA;
                                                     if (!other.isPlayer()) return;
                                                     if (state == DashCrystalState::EMPTY) return;
                                                     Player* player = other.isPlayer();
                                                     player->dashCapacity = true;
                                                     player->stamina = Player::MAX_STAMINA;
                                                     state = DashCrystalState::EMPTY;
                                                     flash_textures_holder.reset();
                                                     full_crystal_texture_holder.reset();
                                                     sound_manager.play(crystal_touch_sound_effects[Random::getInstance().getInt(0, 2)]);
                                                     GameRender::getInstance().shake(axisToVector(collision.axis), 3, 6, 1);
                                                     auto discard = Scheduler::getInstance().schedule([this, &sound_manager](std::shared_ptr<ScheduledEvent> event, float dt) {
                                                         state = DashCrystalState::FULL;
                                                         sound_manager.play(crystal_return_sound_effects[Random::getInstance().getInt(0, 2)]);
                                                     }, RESPAWN_TIME);
                                                 })),
                                                 state(DashCrystalState::FULL), full_crystal_texture_holder(5, "../assets/dashRefill/idle", std::vector<float>(5, 0.2f)),
                                                 main_sprite(EmptyTextures::getInstance().getEmpty({16, 16})),
                                                 flash_textures_holder(6, "../assets/dashRefill/flash", {TIME_FLASH, 0.05, 0.05, 0.05, 0.05, 0.05}),
                                                 flash_sprite(EmptyTextures::getInstance().getEmpty({16, 16})) {
    assert(empty_crystal_texture.loadFromFile("../assets/dashRefill/outline.png"));
    assert(combined_render_texture.resize({sf::VideoMode::getDesktopMode().size}));
}

bool DashCrystal::canCollideWith(const CollidableObject &, Collision collision) const {
    return false;
}

void DashCrystal::updateSprite(float deltaTime) {
    if (!at_top && !at_bottom) {
        time_moving += deltaTime;
        if (time_moving >= 3 * TIME_ONE_PIXEL_MOVE) {
            time_moving = 0;
            if (base_velocity.y > 0) {
                at_bottom = true;
                at_top = false;
            } else {
                at_top = true;
                at_bottom = false;
            }
            base_velocity.y = 0;
        }
    } else {
        time_at_end += deltaTime;
        if (time_at_end >= TIME_STAY_AT_END) {
            time_at_end = 0;
            base_velocity.y = at_top ? 1 / TIME_ONE_PIXEL_MOVE : -1 / TIME_ONE_PIXEL_MOVE;
            at_top = false;
            at_bottom = false;
        }
    }

    if (state == DashCrystalState::EMPTY) {
        sprite.setTexture(empty_crystal_texture);
        return;
    }

    full_crystal_texture_holder.update(deltaTime);
    flash_textures_holder.update(deltaTime);

    main_sprite.setTexture(full_crystal_texture_holder.getCurrentTexture());
    flash_sprite.setTexture(flash_textures_holder.getCurrentTexture());

    combined_render_texture.clear(sf::Color::Transparent);
    main_sprite.setPosition(getPosition());
    flash_sprite.setPosition(getPosition());
    combined_render_texture.draw(main_sprite);
    combined_render_texture.draw(flash_sprite);
    combined_render_texture.display();
    const sf::Sprite combined(combined_render_texture.getTexture());
    sprite = combined;
}



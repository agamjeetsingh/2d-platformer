//
// Created by Agamjeet Singh on 08/09/25.
//

#ifndef DASHCRYSTAL_H
#define DASHCRYSTAL_H
#include <random>

#include "DashCrystalState.h"
#include "entity/CollidableObject.h"
#include "entity/player/Player.h"
#include "events/Collision.h"
#include "events/Listener.h"
#include "utility/EmptyTextures.h"
#include "utility/GameRender.h"

class DashCrystal: public CollidableObject {
public:
    explicit DashCrystal(sf::Vector2f position = {0, 0});

    bool canCollideWith(const CollidableObject &, Collision collision) const override;

    void updateSprite(float deltaTime);

    Listener collision_listener;
    static constexpr float RESPAWN_TIME = 2.5; // In seconds
    static constexpr float TIME_STAY_AT_END = 0.5; // In seconds
    static constexpr float TIME_ONE_PIXEL_MOVE = 0.15; // In seconds
    static constexpr float TIME_FLASH = 2; // In seconds
private:
    bool at_top = true;
    bool at_bottom = false;
    float time_at_end = 0;
    float time_moving = 0;

    DashCrystalState state;

    float time_in_state = 0;
    float time_in_state_flash = 0;

    sf::Texture empty_crystal_texture;
    sf::RenderTexture combined_render_texture;

    size_t curr_sprite_index = 0;
    TexturesHolder full_crystal_texture_holder;
    sf::Sprite main_sprite;

    size_t curr_flash_sprite_index = 0;
    TexturesHolder flash_textures_holder;
    sf::Sprite flash_sprite;

    std::vector<SoundEffect> crystal_touch_sound_effects = {SoundEffect::DIAMOND_TOUCH_01, SoundEffect::DIAMOND_TOUCH_02, SoundEffect::DIAMOND_TOUCH_03};
    std::vector<SoundEffect> crystal_return_sound_effects = {SoundEffect::DIAMOND_RETURN_01, SoundEffect::DIAMOND_RETURN_02, SoundEffect::DIAMOND_RETURN_03};

    static int getRandomInt(int min, int max);
};



#endif //DASHCRYSTAL_H

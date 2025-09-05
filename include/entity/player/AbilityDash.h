//
// Created by Agamjeet Singh on 17/08/25.
//

#ifndef ABILITYDASH_H
#define ABILITYDASH_H
#include <SFML/Graphics.hpp>

#include "DashDirection.h"
#include "Facing.h"

class Player;
#include "entity/player/Ability.h"
#include "utility/ScheduledEvent.h"
#include "utility/SoundManager.h"

class PlayerInputHandler;

class DashSnapshot {
public:
    explicit DashSnapshot(Player& player);

    const sf::Sprite* getSprite();

    void update();

    void setAlpha(uint8_t alpha);

private:
    sf::Texture texture;
    Player& player;
    std::optional<sf::Sprite> sprite;
    std::optional<sf::Sprite> sprite_with_offset;
    Facing facing;
};

class AbilityDash final : public Ability {
public:
    explicit AbilityDash(Player& player);

    [[nodiscard]] bool canPerform() const override;

    void perform() override;

    void cancel() override;

    void startCooldown() override;

    void callDuring() override;

private:
    std::shared_ptr<ScheduledEvent> dash_reset;
    std::shared_ptr<ScheduledEvent> on_cooldown;
    std::shared_ptr<ScheduledEvent> dash_snapshot;
    std::shared_ptr<ScheduledEvent> call_during;
    std::shared_ptr<DashSnapshot> snapshot_first;
    std::shared_ptr<DashSnapshot> snapshot_second;
    std::shared_ptr<DashSnapshot> snapshot_third;
    sf::Vector2f dash_velocity;
};





#endif //ABILITYDASH_H

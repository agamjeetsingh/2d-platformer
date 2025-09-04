//
// Created by Agamjeet Singh on 17/08/25.
//

#include "entity/player/AbilityDash.h"
#include "entity/player/Player.h"
#include "entity/player/PlayerInputHandler.h"
#include "utility/GameRender.h"

AbilityDash::AbilityDash(Player &player):
Ability(player),
snapshot_first(std::make_shared<DashSnapshot>(player)),
snapshot_second(std::make_shared<DashSnapshot>(player)),
snapshot_third(std::make_shared<DashSnapshot>(player)) {}


bool AbilityDash::canPerform() const {
    return !performing && !onCooldown && player.dashCapacity;
}

void AbilityDash::perform() {
    assert(canPerform());
    performing = true;
    player.dashCapacity = false;

    // TODO - Dash Freeze

    auto direction = PlayerInputHandler::getDashDirection(player);

    auto vector = directionToVector(direction);
    dash_velocity = vector * Player::DASH_SPEED;
    player.base_velocity = (player.base_velocity.length() > dash_velocity.length()) ? player.base_velocity : dash_velocity;
    player.disableGravity();

    // Dash Reset
    auto dash_reset_func = [this, direction](std::shared_ptr<ScheduledEvent> event, float deltaTime) {
        player.enableGravity();
        if (direction == DashDirection::LEFT || direction == DashDirection::RIGHT) {
            const float sign = direction == DashDirection::RIGHT ? 1 : -1;
            player.base_velocity.x = sign * Player::DASH_RESET_SPEED_HORIZONTAL;
        } else if (direction == DashDirection::UP_LEFT || direction == DashDirection::UP_RIGHT) {
            const float sign = direction == DashDirection::UP_RIGHT ? 1 : -1;
            player.base_velocity.x = sign * Player::DASH_RESET_SPEED_UP_DIAGONAL;
        } else if (direction == DashDirection::UP || direction == DashDirection::DOWN) {
            const float sign = direction == DashDirection::DOWN ? 1 : -1;
            player.base_velocity.y = sign * Player::DASH_RESET_SPEED_VERTICAL;
        }
        performing = false;
    };
    dash_reset = Scheduler::getInstance().schedule(std::move(dash_reset_func), Player::DASH_SPEED_TIME);
    SoundManager::getInstance().play(directionToSoundEffect(direction));

    auto dash_snapshot_func_first = [this, alpha = static_cast<float>(255)](std::shared_ptr<ScheduledEvent> event, float deltaTime) mutable {
        snapshot_first->setAlpha(static_cast<uint8_t>(alpha));
        if (alpha == 255.0) {
            snapshot_first->updatePosition();
        }

        GameRender::getInstance().draw(snapshot_first);

        alpha *= std::exp(-5 * deltaTime);
        if (alpha <= 1) {
            event->cancelled = true;
            snapshot_first->setAlpha(255);
        }
    };

    auto dash_snapshot_func_second = [this, alpha = static_cast<float>(255)](std::shared_ptr<ScheduledEvent> event, float deltaTime) mutable {
        snapshot_second->setAlpha(static_cast<uint8_t>(alpha));
        if (alpha == 255.0) {
            snapshot_second->updatePosition();
        }

        GameRender::getInstance().draw(snapshot_second);

        alpha *= std::exp(-5 * deltaTime);
        if (alpha <= 1) {
            event->cancelled = true;
            snapshot_second->setAlpha(255);
        }
    };

    auto dash_snapshot_func_third = [this, alpha = static_cast<float>(255)](std::shared_ptr<ScheduledEvent> event, float deltaTime) mutable {
        snapshot_third->setAlpha(static_cast<uint8_t>(alpha));
        if (alpha == 255.0) {
            snapshot_third->updatePosition();
        }

        GameRender::getInstance().draw(snapshot_third);

        alpha *= std::exp(-5 * deltaTime);
        if (alpha <= 1) {
            event->cancelled = true;
            snapshot_third->setAlpha(255);
        }
    };

    dash_snapshot = Scheduler::getInstance().schedule({std::move(dash_snapshot_func_first), Player::DASH_FIRST_SNAPSHOT_TIME, true, 0});
    auto discard = Scheduler::getInstance().schedule({std::move(dash_snapshot_func_second), Player::DASH_SECOND_SNAPSHOT_TIME, true, 0});
    discard = Scheduler::getInstance().schedule({std::move(dash_snapshot_func_third), Player::DASH_THIRD_SNAPSHOT_TIME, true, 0});
}

void AbilityDash::cancel() {
    performing = false;
    if (dash_reset) dash_reset->cancelled = true;
}

void AbilityDash::startCooldown() {
    onCooldown = true;
    auto cooldown_func = [this](std::shared_ptr<ScheduledEvent> event, float deltaTime) {
        onCooldown = false;
    };
    on_cooldown = Scheduler::getInstance().schedule(cooldown_func, Player::DASH_COOLDOWN_TIME);
}

sf::Sprite DashSnapshot::getSprite() const {
    auto scale = (player.facing == Facing::Left) ? sf::Vector2f{-1, 1} : sf::Vector2f{1, 1};
    sf::Sprite sprite_with_offset = sprite.value();
    sprite_with_offset.setScale(scale);
    sprite_with_offset.setPosition(sprite_with_offset.getPosition() + sf::Vector2f{0, -20});
    if (player.facing == Facing::Left) {
        sprite_with_offset.setPosition(sprite_with_offset.getPosition() + sf::Vector2f{-3, 0});
    }
    return sprite_with_offset;
}

DashSnapshot::DashSnapshot(Player &player) : player(player) {
    if (!texture.loadFromFile("../assets/player/dash_transparent00.png")) {
        std::cerr << "Error: Couldn't load from file: " << std::endl;
    }
    sprite = sf::Sprite{texture};
    std::cout << "Loaded texture size: " << texture.getSize().x << "x" << texture.getSize().y << std::endl;
    sf::FloatRect bounds = sprite->getLocalBounds();
    sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y});
}

void DashSnapshot::setAlpha(uint8_t alpha) {
    auto color = sprite->getColor();
    color.a = alpha;
    sprite->setColor(color);
}

void DashSnapshot::updatePosition() {
    sf::Vector2f pos = player.getPosition();
    sprite->setPosition(pos);
}

void AbilityDash::callDuring() {
    player.base_velocity = (player.base_velocity.length() > dash_velocity.length()) ? player.base_velocity : dash_velocity;
}






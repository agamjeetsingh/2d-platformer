cmake_minimum_required(VERSION 3.31)
project(2d-platformer)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 3.0.1 COMPONENTS Graphics Window System Audio REQUIRED)

# Set include directories to avoid ../../../ hell
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(2d-platformer main.cpp
        include/utility/InputManager.h
        src/physics/CollisionsHandler.cpp
        include/physics/CollisionsHandler.h
        src/entity/player/Player.cpp
        include/entity/player/Player.h
        src/entity/CollidableObject.cpp
        include/entity/CollidableObject.h
        src/entity/PhysicsObject.cpp
        include/entity/PhysicsObject.h
        src/physics/Hitbox.cpp
        include/physics/Hitbox.h
        src/events/Collision.cpp
        include/events/Collision.h
        include/entity/CollidableObjectType.h
        include/physics/CollisionAxis.h
        include/entity/player/sprites/PlayerSpriteState.h
        src/entity/player/sprites/PlayerSpriteHandler.cpp
        include/entity/player/sprites/PlayerSpriteHandler.h
        include/entity/player/sprites/Facing.h
        include/entity/sprites/TexturesHolder.h
        src/entity/player/PlayerInputHandler.cpp
        include/entity/player/PlayerInputHandler.h
        include/utility/Scheduler.h
        include/utility/ScheduledEvent.h
        src/events/Contact.cpp
        include/events/Contact.h
        include/physics/ContactAxis.h
        src/physics/ContactsHandler.cpp
        include/physics/ContactsHandler.h
        include/entity/player/ability/DashDirection.h
        src/physics/SpacialHashMap.cpp
        include/physics/SpacialHashMap.h
        include/entity/player/ability/Ability.h
        src/entity/player/ability/AbilityJump.cpp
        include/entity/player/ability/AbilityJump.h
        src/entity/player/ability/AbilityDash.cpp
        include/entity/player/ability/AbilityDash.h
        src/events/EventBus.cpp
        include/events/EventBus.h
        src/events/Event.cpp
        include/events/Event.h
        include/events/EventExecuteTime.h
        src/entity/player/ability/Ability.cpp
        include/events/ListenerPriority.h
        src/events/Listener.cpp
        include/events/Listener.h
        src/events/PlayerOnGround.cpp
        include/events/PlayerOnGround.h
        src/events/PlayerLeftGround.cpp
        include/events/PlayerLeftGround.h
        src/utility/SoundManager.cpp
        include/utility/SoundManager.h
        include/utility/SoundEffect.h
        include/events/PlayerLanded.h
        src/utility/GameRender.cpp
        include/utility/GameRender.h
        include/utility/EmptyTextures.h
)
target_link_libraries(2d-platformer SFML::Graphics SFML::Window SFML::System SFML::Audio)

find_package(SFML 3.0.1 COMPONENTS Graphics Window System Audio REQUIRED)

# Performance test executable
add_executable(performance_test performance_test.cpp
        include/utility/InputManager.h
        src/physics/CollisionsHandler.cpp
        include/physics/CollisionsHandler.h
        src/entity/player/Player.cpp
        include/entity/player/Player.h
        src/entity/CollidableObject.cpp
        include/entity/CollidableObject.h
        src/entity/PhysicsObject.cpp
        include/entity/PhysicsObject.h
        src/physics/Hitbox.cpp
        include/physics/Hitbox.h
        src/events/Collision.cpp
        include/events/Collision.h
        include/entity/CollidableObjectType.h
        include/physics/CollisionAxis.h
        include/entity/player/sprites/PlayerSpriteState.h
        src/entity/player/sprites/PlayerSpriteHandler.cpp
        include/entity/player/sprites/PlayerSpriteHandler.h
        include/entity/player/sprites/Facing.h
        include/entity/sprites/TexturesHolder.h
        src/entity/player/PlayerInputHandler.cpp
        include/entity/player/PlayerInputHandler.h
        include/utility/Scheduler.h
        include/utility/ScheduledEvent.h
        src/events/Contact.cpp
        include/events/Contact.h
        include/physics/ContactAxis.h
        src/physics/ContactsHandler.cpp
        include/physics/ContactsHandler.h
        include/entity/player/ability/DashDirection.h
        src/physics/SpacialHashMap.cpp
        include/physics/SpacialHashMap.h
        include/entity/player/ability/Ability.h
        src/entity/player/ability/AbilityJump.cpp
        include/entity/player/ability/AbilityJump.h
        src/entity/player/ability/AbilityDash.cpp
        include/entity/player/ability/AbilityDash.h
        src/events/EventBus.cpp
        include/events/EventBus.h
        src/events/Event.cpp
        include/events/Event.h
        include/events/EventExecuteTime.h
        src/entity/player/ability/Ability.cpp
        include/events/ListenerPriority.h
        src/events/Listener.cpp
        include/events/Listener.h
        src/events/PlayerOnGround.cpp
        include/events/PlayerOnGround.h
        src/events/PlayerLeftGround.cpp
        include/events/PlayerLeftGround.h
        src/utility/SoundManager.cpp
        include/utility/SoundManager.h
        include/utility/SoundEffect.h
        include/events/PlayerLanded.h
        src/utility/GameRender.cpp
        include/utility/GameRender.h)
target_link_libraries(performance_test SFML::Graphics SFML::Window SFML::System SFML::Audio)

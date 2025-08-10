cmake_minimum_required(VERSION 3.31)
project(game)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 3.0.1 COMPONENTS Graphics Window System REQUIRED)

add_executable(game main.cpp
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
        src/physics/ContactResolution.cpp
        include/physics/ContactResolution.h
        include/entity/PlayerSpriteState.cpp
        include/entity/player/PlayerSpriteState.h
        include/entity/player/sprites/PlayerSpriteHandler.cpp
        include/entity/player/sprites/PlayerSpriteHandler.h
        include/entity/player/Facing.h
        include/entity/player/sprites/PlayerTextures.h
        include/entity/player/sprites/PlayerIdle.h
        include/entity/player/PlayerInputHandler.cpp
        include/entity/player/PlayerInputHandler.h
        include/utility/Scheduler.h
        include/utility/ScheduledEvent.h
        include/events/Contact.cpp
        include/events/Contact.h
        include/physics/ContactAxis.h
        include/physics/ContactsHandler.cpp
        include/physics/ContactsHandler.h
        include/entity/player/PlayerState.cpp
        include/entity/player/PlayerState.h
        include/entity/player/PlayerStateGround.cpp
        include/entity/player/PlayerStateGround.h
        include/entity/player/PlayerStateAir.cpp
        include/entity/player/PlayerStateAir.h
        include/entity/player/PlayerStateClimbing.cpp
        include/entity/player/PlayerStateClimbing.h
        include/entity/player/DashDirection.h
        include/physics/SpacialHashMap.cpp
        include/physics/SpacialHashMap.h
)
target_link_libraries(game SFML::Graphics SFML::Window SFML::System)

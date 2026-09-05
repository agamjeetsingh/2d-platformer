cmake_minimum_required(VERSION 3.28)
project(2d-platformer)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

option(BUILD_BENCHMARKS "Build the Google Benchmark performance suite (benchmarks/)" ${PROJECT_IS_TOP_LEVEL})

if(BUILD_BENCHMARKS)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.1
    )
    FetchContent_MakeAvailable(googlebenchmark)
endif()

find_package(glfw3 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GAME_SOURCES
    include/utility/InputManager.h
    src/physics/2d/CollisionsHandler.cpp
    include/physics/2d/CollisionsHandler.h
    src/entity/player/Player.cpp
    include/entity/player/Player.h
    src/physics/2d/CollidableObject.cpp
    include/physics/2d/CollidableObject.h
    src/physics/2d/PhysicsObject.cpp
    include/physics/2d/PhysicsObject.h
    src/physics/2d/Hitbox.cpp
    include/physics/2d/Hitbox.h
    src/physics/2d/Collision.cpp
    include/physics/2d/Collision.h
    include/physics/2d/CollidableObjectType.h
    include/physics/2d/CollisionAxis.h
    include/entity/player/sprites/PlayerSpriteState.h
    src/entity/player/sprites/PlayerSpriteHandler.cpp
    include/entity/player/sprites/PlayerSpriteHandler.h
    include/entity/player/sprites/Facing.h
    include/entity/sprites/TexturesHolder.h
    src/entity/player/PlayerInputHandler.cpp
    include/entity/player/PlayerInputHandler.h
    include/utility/Scheduler.h
    include/utility/ScheduledEvent.h
    src/physics/2d/Contact.cpp
    include/physics/2d/Contact.h
    include/physics/2d/ContactAxis.h
    src/physics/2d/ContactsHandler.cpp
    include/physics/2d/ContactsHandler.h
    include/entity/player/ability/DashDirection.h
    src/physics/2d/SpacialHashMap.cpp
    include/physics/2d/SpacialHashMap.h
    include/entity/player/ability/Ability.h
    src/entity/player/ability/AbilityJump.cpp
    include/entity/player/ability/AbilityJump.h
    src/entity/player/ability/AbilityDash.cpp
    include/entity/player/ability/AbilityDash.h
    src/events/EventBus.cpp
    include/events/EventBus.h
    src/events/Event.cpp
    include/events/Event.h
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
    src/entity/objects/TouchSwitch.cpp
    include/entity/objects/TouchSwitch.h
    include/entity/objects/TouchSwitchState.h
    include/utility/EmptyTextures.h
    src/entity/objects/OneWayPlatform.cpp
    include/entity/objects/OneWayPlatform.h
    src/entity/objects/KillZone.cpp
    include/entity/objects/KillZone.h
    src/entity/objects/DashCrystal.cpp
    include/entity/objects/DashCrystal.h
    include/entity/objects/DashCrystalState.h
    src/entity/objects/SwapBlock.cpp
    include/entity/objects/SwapBlock.h
    include/events/PlayerDashEvent.h
    src/utility/SoundLoop.cpp
    include/utility/SoundLoop.h
    include/entity/PointParticles.h
    src/entity/PointParticles.cpp
    include/utility/Random.h
    src/entity/SpriteParticles.cpp
    include/entity/SpriteParticles.h
    src/entity/objects/Portal.cpp
    include/entity/objects/Portal.h
    src/utility/ThreadPool.cpp
    include/utility/ThreadPool.h
    src/utility/GameLevel.cpp
    include/utility/GameLevel.h
    src/physics/3d/Collision.cpp
    include/physics/3d/Collision.h
    src/physics/3d/PhysicsObject.cpp
    include/physics/3d/PhysicsObject.h
    src/physics/3d/CollidableObject.cpp
    include/physics/3d/CollidableObject.h
    src/physics/3d/Hitbox.cpp
    include/physics/3d/Hitbox.h
    include/physics/3d/geometry/Vector3f.h
    src/physics/3d/geometry/Box.cpp
    include/physics/3d/geometry/Box.h
    src/physics/3d/CollisionsHandler.cpp
    include/physics/3d/CollisionsHandler.h
    src/physics/3d/SpacialHashMap.cpp
    include/physics/3d/SpacialHashMap.h
    src/physics/3d/Contact.cpp
    include/physics/3d/Contact.h
    src/physics/3d/ContactsHandler.cpp
    include/physics/3d/ContactsHandler.h
)

function(link_game_libraries target)
    target_link_libraries(${target} SFML::Graphics SFML::Window SFML::System SFML::Audio)
    target_link_libraries(${target} nlohmann_json::nlohmann_json)
    target_link_libraries(${target} glfw glm::glm)
endfunction()

function(create_game_executable target_name main_source)
    add_executable(${target_name} ${main_source} ${GAME_SOURCES})
    link_game_libraries(${target_name})
endfunction()

if(PROJECT_IS_TOP_LEVEL)
    create_game_executable(2d-platformer main.cpp)
    
    add_custom_command(TARGET 2d-platformer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:2d-platformer>/assets
    )

    create_game_executable(2d-platformer-sanitized main.cpp)
    target_compile_options(2d-platformer-sanitized PRIVATE -fsanitize=address,undefined -g)
    target_link_options(2d-platformer-sanitized PRIVATE -fsanitize=address,undefined)

    create_game_executable(2d-platformer-tsan main.cpp)
    target_compile_options(2d-platformer-tsan PRIVATE -fsanitize=thread -g)
    target_link_options(2d-platformer-tsan PRIVATE -fsanitize=thread)

endif() # End of PROJECT_IS_TOP_LEVEL

add_library(game_lib STATIC ${GAME_SOURCES})

target_include_directories(game_lib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

link_game_libraries(game_lib)

if(BUILD_BENCHMARKS)
    add_executable(benchmarks benchmarks/CollisionBenchmark.cpp)
    target_link_libraries(benchmarks PRIVATE game_lib benchmark::benchmark)
endif()

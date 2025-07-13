cmake_minimum_required(VERSION 3.31)
project(game)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 3.0.1 COMPONENTS Graphics Window System REQUIRED)

add_executable(game main.cpp
        include/utility/InputManager.h
        src/physics/Collider.cpp
        include/physics/Collider.h
        src/physics/ColliderBodies.cpp
        include/physics/ColliderBodies.h
        include/entity/Player.cpp
        include/entity/Player.h
)
target_link_libraries(game SFML::Graphics SFML::Window SFML::System)

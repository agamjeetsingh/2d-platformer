//
// Created by Agamjeet Singh on 17/09/25.
//

#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <SFML/Graphics.hpp>

#include "GameRender.h"
#include "entity/objects/DashCrystal.h"

using json = nlohmann::json;

namespace fs = std::filesystem;

struct Tileset {
    std::string source;
    int firstgid;

    int tileWidth;
    int tileHeight;
    int columns;

    sf::Texture texture;

    Tileset(const std::string& source, int firstgid);

    sf::Sprite getSprite(int localID) const;
};

inline fs::path resolveRelative(const fs::path& baseFile, const std::string& relativePath) {
    const fs::path baseDir = baseFile.parent_path();
    const fs::path rel = relativePath;
    return fs::weakly_canonical(baseDir / rel); // normalizes ".." and "."
}


class GameLevel {
public:
    explicit GameLevel(CollisionsHandler& collisions_handler, SoundManager<SoundEffect>& sound_manager);

    void load(const std::string& path);
private:
    std::vector<std::any> objects;
    std::vector<Tileset> tilesets;
    CollisionsHandler& collisions_handler;
    SoundManager<SoundEffect>& sound_manager;
};



#endif //GAMELEVEL_H

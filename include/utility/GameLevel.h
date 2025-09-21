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
#include "entity/CollidableObject.h"
#include "entity/CollidableObjectType.h"
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

    Tileset(const std::string& source, int firstgid) : source(source), firstgid(firstgid) {
        std::ifstream tileSetFile(source);
        json tileSet;
        tileSetFile >> tileSet;

        tileWidth = tileSet["tilewidth"];
        tileHeight = tileSet["tileheight"];
        columns = tileSet["columns"];

        std::string imagePath = tileSet["image"];
        fs::path tilesetDir = fs::path(source).parent_path();
        fs::path imageFile = tilesetDir / imagePath;
        std::cout << imageFile.lexically_normal().string() << std::endl;
        assert(texture.loadFromFile(imageFile.lexically_normal().string()));

        if (!tileSet.contains("tiles")) return;

        for (auto& [idStr, tileData] : tileSet["tiles"].items()) {
            int id = std::stoi(idStr);
            if (tileData.contains("properties")) {
                for (auto& p : tileData["properties"]) {
                    if (p["name"] == "type" && p["value"].get<std::string>() == "") {
                        // prop.collidable = true;
                    }
                }
            }
            // tileset.properties[id] = prop;
        }
    }

    sf::Sprite getSprite(int localID) const {
        const int tu = localID % columns; // column index
        const int tv = localID / columns; // row index
        sf::Sprite sprite(texture);
        sprite.setTextureRect(sf::IntRect( {tu * tileWidth, tv * tileHeight}, {tileWidth, tileHeight}));
        return sprite;
    }
};

inline fs::path resolveRelative(const fs::path& baseFile, const std::string& relativePath) {
    fs::path baseDir = baseFile.parent_path();
    fs::path rel = relativePath;
    return fs::weakly_canonical(baseDir / rel); // normalizes ".." and "."
}


class GameLevel {
public:
    void load() {
        std::string path = "../assets/levels/map.json";
        std::ifstream mapFile(path);
        json map;
        mapFile >> map;
        size_t num_tilesets = map["tilesets"].size();
        tilesets.reserve(num_tilesets);
        for (auto& tsJson : map["tilesets"]) {
            std::string relativeSource = tsJson["source"];
            fs::path tilesetPath = resolveRelative(path, relativeSource);
            tilesets.emplace_back(tilesetPath.string(), tsJson["firstgid"]);
        }

        int width = map["width"];
        int height = map["height"];
        auto layers = map["layers"];

        auto data = layers[0]["data"];
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int gid = data[y * width + x];
                if (gid == 0) continue; // empty

                const Tileset* tileset = nullptr;
                for (auto& ts : tilesets) {
                    if (gid >= ts.firstgid) {
                        tileset = &ts;
                    }
                }
                assert(tileset);

                int localID = gid - tileset->firstgid;

                sf::Sprite sprite = tileset->getSprite(localID);
                sprite.setPosition({static_cast<float>(x * tileset->tileWidth), static_cast<float>(y * tileset->tileHeight)});

                auto tile = std::make_shared<CollidableObject>(std::vector<sf::FloatRect>{{{0, 0}, {static_cast<float>(tileset->tileWidth), static_cast<float>(tileset->tileHeight)}}}, sprite, sprite.getPosition(), CollidableObjectType::Immovable);
                GameRender::getInstance().registerDrawable(tile);
                objects.emplace_back(tile);
            }
        }

        auto object_layer = layers[1]["objects"];
        for (const auto& object: object_layer) {
            const auto& properties = object["properties"];
            for (const auto& prop : properties) {
                if (prop["name"].get<std::string>() == "dash_crystal" &&
                    prop["value"].get<bool>())
                {
                    auto dash_crystal = std::make_shared<DashCrystal>(
                        sf::Vector2f{object["x"].get<float>(), object["y"].get<float>()});
                    GameRender::getInstance().registerDrawable(dash_crystal);
                    objects.emplace_back(dash_crystal);
                }
            }
        }
    }
private:
    std::vector<std::any> objects;
    std::vector<Tileset> tilesets;
};



#endif //GAMELEVEL_H

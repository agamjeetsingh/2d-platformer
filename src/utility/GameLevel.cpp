//
// Created by Agamjeet Singh on 17/09/25.
//

#include "../../include/utility/GameLevel.h"

Tileset::Tileset(const std::string &source, int firstgid) : source(source), firstgid(firstgid) {
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
}

sf::Sprite Tileset::getSprite(int localID) const {
    const int tu = localID % columns; // column index
    const int tv = localID / columns; // row index
    sf::Sprite sprite(texture);
    sprite.setTextureRect(sf::IntRect( {tu * tileWidth, tv * tileHeight}, {tileWidth, tileHeight}));
    return sprite;
}

GameLevel::GameLevel(CollisionsHandler &collisions_handler, SoundManager<SoundEffect> &sound_manager): collisions_handler(collisions_handler), sound_manager(sound_manager) {}

void GameLevel::load(const std::string &path) {
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

    for (const auto& layer : layers) {
        std::string type = layer["type"];

        if (type == "tilelayer") {
            auto data = layer["data"];
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    int gid = data[y * width + x];
                    if (gid == 0) continue;

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

                    auto tile = std::make_shared<CollidableObject>(
                        std::vector<sf::FloatRect>{{{0,0}, {static_cast<float>(tileset->tileWidth), static_cast<float>(tileset->tileHeight)}}},
                        sprite,
                        sprite.getPosition(),
                        CollidableObjectType::Immovable
                    );
                    collisions_handler.addObject(*tile);
                    GameRender::getInstance().registerDrawable(tile);
                    objects.emplace_back(tile);
                }
            }
        }
        else if (type == "objectgroup") {
            for (const auto& object : layer["objects"]) {
                const auto& properties = object["properties"];
                for (const auto& prop : properties) {
                    if (prop["name"].get<std::string>() == "dash_crystal" &&
                        prop["value"].get<bool>())
                    {
                        auto dash_crystal = std::make_shared<DashCrystal>(
                            sf::Vector2f{object["x"].get<float>(), object["y"].get<float>()},
                            sound_manager
                        );
                        collisions_handler.addObject(*dash_crystal);
                        GameRender::getInstance().registerDrawable(dash_crystal);
                        objects.emplace_back(dash_crystal);
                    }
                }
            }
        }
    }
}

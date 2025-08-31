//
// Created by Agamjeet Singh on 26/08/25.
//

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <iostream>
#include <unordered_map>
#include <vector>
#include <SFML/Audio.hpp>

#include "SoundEffect.h"

class SoundManager {
public:
    static SoundManager& getInstance();

    void play(SoundEffect sound_effect, float volume = 100);

    static void loadBuffers() { getInstance(); }
private:
    std::unordered_map<SoundEffect, sf::SoundBuffer> buffers = {};
    std::vector<sf::Sound> sounds;
    SoundManager();
};



#endif //SOUNDMANAGER_H

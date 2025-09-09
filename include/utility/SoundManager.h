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

    std::shared_ptr<sf::Sound> play(SoundEffect sound_effect, bool loop = false, float volume = 100);

    static void loadBuffers() { getInstance(); }

    float getDuration(SoundEffect sound_effect) {
        if (!buffers.contains(sound_effect)) return 0;
        return buffers[sound_effect].getDuration().asSeconds();
    }
private:
    std::unordered_map<SoundEffect, sf::SoundBuffer> buffers = {};
    std::vector<std::shared_ptr<sf::Sound>> sounds;
    SoundManager();
};



#endif //SOUNDMANAGER_H

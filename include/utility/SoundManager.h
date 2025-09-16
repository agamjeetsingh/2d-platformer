//
// Created by Agamjeet Singh on 26/08/25.
//

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>
#include <SFML/Audio.hpp>

#include "SoundEffect.h"

class SoundManager {
public:
    static SoundManager& getInstance();

    std::shared_ptr<sf::Sound> play(SoundEffect sound_effect, bool loop = false, float volume = 100);

    static void loadBuffers() { getInstance(); }

    float getDuration(SoundEffect sound_effect);

    void removeExpiredSounds();

private:
    std::unordered_map<SoundEffect, sf::SoundBuffer> buffers = {};
    std::map<float, std::pair<std::shared_ptr<sf::Sound>, SoundEffect>> sounds;
    std::vector<float> sound_lifetimes; // In seconds
    sf::Clock clock;

    std::mutex sound_mutex;

    SoundManager();
};



#endif //SOUNDMANAGER_H

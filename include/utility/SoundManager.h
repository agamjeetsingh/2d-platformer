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
    std::shared_ptr<sf::Sound> play(SoundEffect sound_effect, bool loop = false, float volume = 100);

    [[nodiscard]] float getDuration(SoundEffect sound_effect) const;

    void removeExpiredSounds();

    bool registerSoundEffect(const SoundEffect effect, const std::string& filename) {
        static bool first_time = true;
        const bool success = buffers[effect].loadFromFile(filename);
        if (!success) {
            std::cerr << "Error: Could not load " << filename << std::endl;
        }

        if (first_time) {
            sf::Sound dummy(buffers.begin()->second);
            dummy.play();
            dummy.stop();
        }
        first_time = false;

        return success;
    }

private:
    std::unordered_map<SoundEffect, sf::SoundBuffer> buffers = {};
    std::map<float, std::pair<std::shared_ptr<sf::Sound>, SoundEffect>> sounds;
    std::vector<float> sound_lifetimes; // In seconds
    sf::Clock clock;

    std::mutex sound_mutex;
};



#endif //SOUNDMANAGER_H

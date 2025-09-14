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

    float getDuration(SoundEffect sound_effect) {
        if (!buffers.contains(sound_effect)) return 0;
        return buffers[sound_effect].getDuration().asSeconds();
    }

    void removeExpiredSounds() {
        const float curr_time = clock.getElapsedTime().asSeconds();
        for (auto it = sounds.begin(); it != sounds.end();) {
            if (curr_time > it->first) {
                const auto& [sound_ptr, sound_effect] = it->second;
                if (sound_ptr->getStatus() == sf::Sound::Status::Stopped) {
                    it = sounds.erase(it);
                } else {
                    it = sounds.erase(it);
                    sounds.insert({curr_time + getDuration(sound_effect), {sound_ptr, sound_effect}});
                }
            } else {
                break;
            }
        }
    }

private:
    std::unordered_map<SoundEffect, sf::SoundBuffer> buffers = {};
    std::map<float, std::pair<std::shared_ptr<sf::Sound>, SoundEffect>> sounds;
    std::vector<float> sound_lifetimes; // In seconds
    sf::Clock clock;
    SoundManager();
};



#endif //SOUNDMANAGER_H

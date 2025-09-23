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

template <typename Key>
class SoundManager {
public:
    std::shared_ptr<sf::Sound> play(Key sound_effect, const bool loop = false, float volume = 100) {
        removeExpiredSounds();
        volume = std::clamp(volume, 0.f, 100.f);

        if (!buffers.contains(sound_effect)) return {};
        const sf::SoundBuffer& buffer = buffers.at(sound_effect);
        float expiration_time = clock.getElapsedTime().asSeconds() + getDuration(sound_effect);
        auto shared_ptr = std::make_shared<sf::Sound>(buffer);
        {
            std::lock_guard lock{sound_mutex};
            sounds.emplace(expiration_time, std::make_pair(shared_ptr, sound_effect));
        }
        shared_ptr->setVolume(volume);
        shared_ptr->setLooping(loop);
        shared_ptr->play();
        return shared_ptr;
    }

    [[nodiscard]] float getDuration(Key sound_effect) const {
        if (!buffers.contains(sound_effect)) return 0;
        return buffers.at(sound_effect).getDuration().asSeconds();
    }

    void removeExpiredSounds() {
        const float curr_time = clock.getElapsedTime().asSeconds();
        std::lock_guard lock{sound_mutex};
        for (auto it = sounds.begin(); it != sounds.end();) {
            if (curr_time > it->first) {
                const auto [sound_ptr, sound_effect] = it->second;
                if (!sound_ptr || sound_ptr->getStatus() == sf::Sound::Status::Stopped) {
                    it = sounds.erase(it);
                } else {
                    it = sounds.erase(it);
                    sounds.emplace(curr_time + getDuration(sound_effect), std::make_pair(sound_ptr, sound_effect));
                }
            } else {
                break;
            }
        }
    }

    bool registerSoundEffect(const Key effect, const std::string& filename) {
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
    std::unordered_map<Key, sf::SoundBuffer> buffers = {};
    std::map<float, std::pair<std::shared_ptr<sf::Sound>, Key>> sounds;
    std::vector<float> sound_lifetimes; // In seconds
    sf::Clock clock;

    std::mutex sound_mutex;
};



#endif //SOUNDMANAGER_H

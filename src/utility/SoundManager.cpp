//
// Created by Agamjeet Singh on 26/08/25.
//

#include "../../include/utility/SoundManager.h"

std::shared_ptr<sf::Sound> SoundManager::play(SoundEffect sound_effect, bool loop, float volume) {
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

float SoundManager::getDuration(SoundEffect sound_effect) const {
    if (!buffers.contains(sound_effect)) return 0;
    return buffers.at(sound_effect).getDuration().asSeconds();
}


void SoundManager::removeExpiredSounds() {
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




//
// Created by Agamjeet Singh on 26/08/25.
//

#include "../../include/utility/SoundManager.h"

SoundManager &SoundManager::getInstance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::play(SoundEffect sound_effect, float volume) {
    volume = std::clamp(volume, 0.f, 100.f);

    if (!buffers.contains(sound_effect)) return;
    const sf::SoundBuffer& buffer = buffers.at(sound_effect);

    for (auto& sound : sounds) {
        if (sound.getStatus() != sf::Sound::Status::Playing) {
            sound.setBuffer(buffer);
            sound.setVolume(volume);
            sound.play();
            return;
        }
    }

    sounds.emplace_back(buffer);
    sounds.back().setVolume(volume);
    sounds.back().play();
}

SoundManager::SoundManager() {
    if (!buffers[SoundEffect::DASH_LEFT].loadFromFile("../assets/player/audio/dash_red_left.wav")) {
        std::cerr << "Error: Could not load ../assets/player/audio/dash_red_left.wav\n";
    }
    if (!buffers[SoundEffect::DASH_RIGHT].loadFromFile("../assets/player/audio/dash_red_right.wav")) {
        std::cerr << "Error: Could not load ../assets/player/audio/dash_red_right.wav\n";
    }
    if (!buffers[SoundEffect::LAND].loadFromFile("../assets/player/audio/land_00_dirt_01.wav")) {
        std::cerr << "Error: Could not load ../assets/player/audio/land_00_dirt_01.wav\n";
    }
    if (!buffers[SoundEffect::TOUCH_SWITCH_ACTIVATE].loadFromFile("../assets/touchSwitch/touchswitch_any.wav")) {
        std::cerr << "Error: Could not load ../assets/touchSwitch/touchswitch_any.wav\n";
    }
    if (!buffers[SoundEffect::TOUCH_SWITCH_ALL_ACTIVATE].loadFromFile("../assets/touchSwitch/touchswitch_last.wav")) {
        std::cerr << "Error: Could not load ../assets/touchSwitch/touchswitch_last.wav\n";
    }
    if (!buffers[SoundEffect::DEATH].loadFromFile("../assets/player/death.wav")) {
        std::cerr << "Error: Could not load ../assets/player/death.wav\n";
    }
    if (!buffers[SoundEffect::DIAMOND_RETURN_01].loadFromFile("../assets/dashRefill/diamond_return_01.wav")) {
        std::cerr << "Error: Could not load ../assets/dashRefill/diamond_return_01.wav\n";
    }
    if (!buffers[SoundEffect::DIAMOND_RETURN_02].loadFromFile("../assets/dashRefill/diamond_return_02.wav")) {
        std::cerr << "Error: Could not load ../assets/dashRefill/diamond_return_02.wav\n";
    }
    if (!buffers[SoundEffect::DIAMOND_RETURN_03].loadFromFile("../assets/dashRefill/diamond_return_03.wav")) {
        std::cerr << "Error: Could not load ../assets/dashRefill/diamond_return_03.wav\n";
    }
    if (!buffers[SoundEffect::DIAMOND_TOUCH_01].loadFromFile("../assets/dashRefill/diamond_touch_01.wav")) {
        std::cerr << "Error: Could not load ../assets/dashRefill/diamond_touch_01.wav\n";
    }
    if (!buffers[SoundEffect::DIAMOND_TOUCH_02].loadFromFile("../assets/dashRefill/diamond_touch_02.wav")) {
        std::cerr << "Error: Could not load ../assets/dashRefill/diamond_touch_02.wav\n";
    }
    if (!buffers[SoundEffect::DIAMOND_TOUCH_03].loadFromFile("../assets/dashRefill/diamond_touch_03.wav")) {
        std::cerr << "Error: Could not load ../assets/dashRefill/diamond_touch_03.wav\n";
    }

    sf::Sound dummy(buffers.begin()->second);
    dummy.play();
    dummy.stop();
}



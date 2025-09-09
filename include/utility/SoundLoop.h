//
// Created by Agamjeet Singh on 09/09/25.
//

#ifndef SOUNDLOOP_H
#define SOUNDLOOP_H
#include "Scheduler.h"
#include "SoundEffect.h"
#include "SoundManager.h"


class SoundLoop {
public:
    SoundLoop(SoundEffect loop_begin, SoundEffect loop, SoundEffect loop_end);

    void start();

    void end();

    void abort() const;

private:
    SoundEffect loop_begin;
    SoundEffect loop;
    SoundEffect loop_end;
    std::shared_ptr<ScheduledEvent> start_loop_event;
    std::shared_ptr<sf::Sound> begin_sound;
    std::shared_ptr<sf::Sound> loop_sound;
    std::shared_ptr<sf::Sound> end_sound;
};



#endif //SOUNDLOOP_H

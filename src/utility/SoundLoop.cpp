//
// Created by Agamjeet Singh on 09/09/25.
//

#include "../../include/utility/SoundLoop.h"

SoundLoop::SoundLoop(SoundManager<SoundEffect>& sound_manager, SoundEffect loop_begin, SoundEffect loop, SoundEffect loop_end) : sound_manager(sound_manager), loop_begin(loop_begin), loop(loop), loop_end(loop_end) {}

void SoundLoop::start() {
    begin_sound = sound_manager.play(loop_begin);
    if (start_loop_event) start_loop_event->cancel();
    start_loop_event = Scheduler::getInstance().schedule([this](std::shared_ptr<ScheduledEvent> event, float dt) {
        if (loop_sound) loop_sound->stop();
        loop_sound = sound_manager.play(loop, true);
    }, sound_manager.getDuration(loop_begin));
}

void SoundLoop::end() {
    if (start_loop_event) {
        start_loop_event->cancel();
    }
    if (loop_sound) {
        loop_sound->stop();
    }
    end_sound = sound_manager.play(loop_end);
}

void SoundLoop::abort() const {
    if (start_loop_event) start_loop_event->cancel();
    if (begin_sound) begin_sound->stop();
    if (loop_sound) loop_sound->stop();
    if (end_sound) end_sound->stop();
}



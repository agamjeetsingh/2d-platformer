//
// Created by Agamjeet Singh on 26/07/25.
//

#ifndef SCHEDULEDEVENT_H
#define SCHEDULEDEVENT_H
#include <functional>
#include <utility>


struct ScheduledEvent {
    float timeRemaining;
    std::function<void()> callback;
    bool repeat;
    float interval; // interval = 0 would mean run once every frame, events run a maximum of once per frame
    float spentTime = 0;
    float maxTime;

    ScheduledEvent(std::function<void()> cb, float time, bool repeat = false, float interval = 0, float maxTime = -1)
        : timeRemaining(time), callback(std::move(cb)), repeat(repeat), interval(interval), maxTime(maxTime) {
        assert(repeat || (maxTime == -1)); // If repeat is false then maxTime must be -1
        assert(!repeat || (maxTime > 0)); // If repeat is true then maxTime must be positive
    }
};



#endif //SCHEDULEDEVENT_H

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

    bool cancelled = false;

    // TODO - Overload constructor to make it simpler. Separate constructors for repeating and non-repeating events.

    ScheduledEvent(std::function<void()> cb, float time, bool repeat = false, float interval = 0, float maxTime = -1)
        : timeRemaining(time), callback([this, cb]() {
            if (!this->cancelled) {
                cb();
            }
        }), repeat(repeat), interval(interval), maxTime(maxTime) {
        assert(repeat || (maxTime == -1)); // If repeat is false then maxTime must be -1
        assert(!repeat || (maxTime > 0 || maxTime == -1)); // If repeat is true then maxTime must be positive or default -1
    }
};



#endif //SCHEDULEDEVENT_H

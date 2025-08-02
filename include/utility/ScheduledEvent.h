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
    float interval;

    ScheduledEvent(std::function<void()> cb, float time, bool repeat = false, float interval = 0)
        : timeRemaining(time), callback(std::move(cb)), repeat(repeat), interval(interval) {}
};



#endif //SCHEDULEDEVENT_H

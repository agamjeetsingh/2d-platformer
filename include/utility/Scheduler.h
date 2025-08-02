//
// Created by Agamjeet Singh on 26/07/25.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "ScheduledEvent.h"

class Scheduler {
public:
    void schedule(std::function<void()> callback, float delaySeconds) {
        events.emplace_back(std::move(callback), delaySeconds);
    }

    static Scheduler& getInstance() {
        static Scheduler scheduler;
        return scheduler;
    }

    void update(float dt) {
        for (auto it = events.begin(); it != events.end(); ) {
            it->timeRemaining -= dt;
            if (it->timeRemaining <= 0.0f) {
                it->callback();
                if (it->repeat) {
                    it->timeRemaining = it->interval;
                    ++it;
                } else {
                    it = events.erase(it);
                }
            } else {
                ++it;
            }
        }
    }

    void cancelAllEvents() {
        events = {};
    }
private:
    std::vector<ScheduledEvent> events;
};



#endif //SCHEDULER_H

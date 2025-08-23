//
// Created by Agamjeet Singh on 26/07/25.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "ScheduledEvent.h"

class Scheduler {
public:
    std::shared_ptr<ScheduledEvent> schedule(std::function<void()> callback, float delaySeconds) {
        const auto event_ptr = std::make_shared<ScheduledEvent>(std::move(callback), delaySeconds);
        events.push_back(event_ptr);
        return event_ptr;
    }

    std::shared_ptr<ScheduledEvent> schedule(ScheduledEvent event) {
        const auto event_ptr = std::make_shared<ScheduledEvent>(std::move(event));
        events.push_back(event_ptr);
        return event_ptr;
    }

    static Scheduler& getInstance() {
        static Scheduler scheduler;
        return scheduler;
    }

    void update(float dt) {
        for (auto it = events.begin(); it != events.end(); ) {
            if (it->get()->cancelled) {
                it = events.erase(it);
                continue;
            }
            it->get()->timeRemaining -= dt;
            it->get()->spentTime += dt;
            if (it->get()->timeRemaining <= 0.0f) {
                it->get()->callback();
                if (it->get()->repeat) {
                    if (it->get()->maxTime != -1 && it->get()->maxTime <= it->get()->spentTime) {
                        it = events.erase(it);
                    } else {
                        it->get()->timeRemaining = it->get()->interval;
                        ++it;
                    }
                } else {
                    it = events.erase(it);
                }
            } else {
                if (it->get()->maxTime != -1 && it->get()->maxTime <= it->get()->spentTime) {
                    it = events.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    void cancelAllEvents() {
        events = {};
    }
private:
    std::vector<std::shared_ptr<ScheduledEvent>> events;
};



#endif //SCHEDULER_H

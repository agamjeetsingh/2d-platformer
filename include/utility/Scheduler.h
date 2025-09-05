//
// Created by Agamjeet Singh on 26/07/25.
//

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include "ScheduledEvent.h"

class Scheduler {
public:
    [[nodiscard]] std::shared_ptr<ScheduledEvent> schedule(std::function<void(std::shared_ptr<ScheduledEvent>, float)> callback, float delaySeconds) {
        const auto event_ptr = std::make_shared<ScheduledEvent>(std::move(callback), delaySeconds);
        event_ptr->setup_callback();
        if (updating) {
            eventsBuffer.push_back(event_ptr);
        } else {
            events.push_back(event_ptr);
        }
        return event_ptr;
    }

    [[nodiscard]] std::shared_ptr<ScheduledEvent> schedule(ScheduledEvent event) {
        const auto event_ptr = std::make_shared<ScheduledEvent>(std::move(event));
        event_ptr->setup_callback();
        if (updating) {
            eventsBuffer.push_back(event_ptr);
        } else {
            events.push_back(event_ptr);
        }
        return event_ptr;
    }

    static Scheduler& getInstance() {
        static Scheduler scheduler;
        return scheduler;
    }

    void update(float dt) {
        updating = true;
        for (auto it = events.begin(); it != events.end(); ) {
            if (std::ranges::any_of(events, [](const std::shared_ptr<ScheduledEvent>& event){return !event; })) {

            }
            assert(it->get());

            if (it->get()->cancelled) {
                it = events.erase(it);
                continue;
            }
            it->get()->timeRemaining -= dt;
            it->get()->spentTime += dt;
            if (it->get()->timeRemaining <= 0.0f) {
                it->get()->callback(dt);
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

        updating = false;
        events.insert(events.end(), eventsBuffer.begin(), eventsBuffer.end());
        eventsBuffer.clear();
    }

    void cancelAllEvents() {
        events.clear();
        eventsBuffer.clear();
    }
private:
    std::vector<std::shared_ptr<ScheduledEvent>> eventsBuffer;
    std::vector<std::shared_ptr<ScheduledEvent>> events;
    bool updating = false;
};



#endif //SCHEDULER_H

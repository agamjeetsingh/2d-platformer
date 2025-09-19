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
        const std::unique_lock lock(update_mtx, std::try_to_lock);
        if (lock) {
            std::lock_guard events_lock{events_mtx};
            events.push_back(event_ptr);
        } else {
            std::lock_guard events_buffer_lock{events_buffer_mtx};
            eventsBuffer.push_back(event_ptr);
        }
        return event_ptr;
    }

    template<typename... Args>
    [[nodiscard]] std::shared_ptr<ScheduledEvent> schedule(Args&&... args) {
        const auto event_ptr = std::make_shared<ScheduledEvent>(std::forward<Args>(args)...);
        event_ptr->setup_callback();
        const std::unique_lock lock(update_mtx, std::try_to_lock);
        if (lock) {
            std::lock_guard events_lock{events_mtx};
            events.push_back(event_ptr);
        } else {
            std::lock_guard events_buffer_lock{events_buffer_mtx};
            eventsBuffer.push_back(event_ptr);
        }
        return event_ptr;
    }

    static Scheduler& getInstance() {
        static Scheduler scheduler;
        return scheduler;
    }

    void update(float dt) {
        std::lock_guard update_lock{update_mtx};
        std::lock_guard events_lock{events_mtx};
        for (auto it = events.begin(); it != events.end(); ) {
            assert(it->get());

            if (it->get()->is_cancelled()) {
                it = events.erase(it);
                continue;
            }
            it->get()->timeRemaining -= dt;
            it->get()->spentTime += dt;
            if (it->get()->timeRemaining <= 0.0f) {
                it->get()->callback(dt);
                if (it->get()->repeat) {
                    it->get()->timeRemaining = it->get()->interval;
                    ++it;
                } else {
                    it = events.erase(it);
                }
            } else {
                ++it;
            }
        }
        std::lock_guard events_buffer_lock{events_buffer_mtx};
        events.insert(events.end(), eventsBuffer.begin(), eventsBuffer.end());
        eventsBuffer.clear();
    }

    void cancelAllEvents() {
        std::lock_guard update_lock{update_mtx};
        std::lock_guard events_lock{events_mtx};
        std::lock_guard events_buffer_lock{events_buffer_mtx};
        events.clear();
        eventsBuffer.clear();
    }

private:
    std::vector<std::shared_ptr<ScheduledEvent>> eventsBuffer;
    std::vector<std::shared_ptr<ScheduledEvent>> events;
    mutable std::mutex events_buffer_mtx;
    mutable std::mutex events_mtx;
    mutable std::mutex update_mtx;
};



#endif //SCHEDULER_H

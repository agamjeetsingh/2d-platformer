//
// Created by Agamjeet Singh on 26/07/25.
//

#ifndef SCHEDULEDEVENT_H
#define SCHEDULEDEVENT_H
#include <functional>
#include <cassert>
#include <utility>
#include <memory>


struct ScheduledEvent : std::enable_shared_from_this<ScheduledEvent> {
    float timeRemaining;
    std::function<void(float)> callback;
    float spentTime = 0;

    const std::function<void(std::shared_ptr<ScheduledEvent>, float)> user_callback;
    const bool repeat;
    const float interval; // interval = 0 would mean run once every frame, events run a maximum of once per frame
    const float maxTime;

    ScheduledEvent(std::function<void(std::shared_ptr<ScheduledEvent>, float)> cb, float time, bool repeat = false, float interval = 0)
        : timeRemaining(time), user_callback(std::move(cb)), repeat(repeat), interval(interval), maxTime(-1) {}

    void setup_callback() {
        callback = [self = shared_from_this()](float deltaTime) {
            if (!self->cancelled) {
                self->user_callback(self, deltaTime);
            }
        };
    }

    void cancel() {
        cancelled.store(true, std::memory_order_release);
    }

    [[nodiscard]] bool is_cancelled() const {
        return cancelled.load(std::memory_order_acquire);
    }

private:
    std::atomic_bool cancelled = false;
};



#endif //SCHEDULEDEVENT_H

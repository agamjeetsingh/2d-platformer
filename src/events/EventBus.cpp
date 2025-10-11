//
// Created by Agamjeet Singh on 17/08/25.
//

#include "../../include/events/EventBus.h"

#include <cassert>

#include "events/Listener.h"
#include <mutex>

EventBus &EventBus::getInstance() {
    static EventBus instance;
    return instance;
}

void EventBus::emit(Event event) {
    if (event.execute_time == EventExecuteTime::NOW) {
        executeNow(event);
        return;
    }
    std::lock_guard events_lock{events_mtx};
    events_by_execution[event.execute_time].push(std::move(event));
}

void EventBus::executeNow(Event event) {
    assert(event.execute_time == EventExecuteTime::NOW);
    std::vector<Listener> snapshot;
    {
        std::lock_guard listeners_lock{listeners_mtx};
        const auto& multiset = listeners[event.type_index];
        snapshot.reserve(multiset.size());
        std::ranges::copy(multiset, std::back_inserter(snapshot));
    }
    for (auto& listener : snapshot) {
        listener.call(&event);
    }
}
// TODO - Get all listeners before while loop. But have to change it so they are all shared_ptrs instead
void EventBus::execute(EventExecuteTime time) {
    std::queue<Event> events;
    {
        std::lock_guard events_lock{events_mtx};
        std::swap(events, events_by_execution[time]);
    }
    while (!events.empty()) {
        Event event = events.front();
        events.pop();
        std::vector<Listener> snapshot;
        {
            std::lock_guard listeners_lock{listeners_mtx};
            const auto& multiset = listeners[event.type_index];
            snapshot.reserve(multiset.size());
            std::ranges::copy(multiset, std::back_inserter(snapshot));
        }
        for (auto& listener : snapshot) {
            listener.call(&event);
        }
    }
}

void EventBus::registerListener(Listener listener) {
    std::unique_lock listeners_lock(listeners_mtx);
    listeners[listener.type_index].insert(std::move(listener));
}

void EventBus::clear() {
    std::unique_lock events_lock{events_mtx};
    std::unique_lock listeners_lock{listeners_mtx};
    listeners.clear();
    events_by_execution.clear();
}

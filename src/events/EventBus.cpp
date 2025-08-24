//
// Created by Agamjeet Singh on 17/08/25.
//

#include "../../include/events/EventBus.h"

#include <cassert>

#include "events/Listener.h"

EventBus &EventBus::getInstance() {
    static EventBus instance;
    return instance;
}

void EventBus::emit(Event event) {
    if (!listeners.contains(event.type_index)) return;

    if (event.execute_time == EventExecuteTime::NOW) {
        executeNow(event);
        return;
    }
    events_by_execution[event.execute_time].push(std::move(event));
}


void EventBus::executeNow(Event event) const {
    assert(event.execute_time == EventExecuteTime::NOW);
    auto& multiset = listeners.at(event.type_index);
    for (const Listener& listener: multiset) {
        listener.call(&event);
    }
}

void EventBus::execute(EventExecuteTime time) {
    auto& events = events_by_execution[time];
    while (!events.empty()) {
        Event event = events.front();
        events.pop();
        auto& multiset = listeners[event.type_index];
        for (const Listener& listener: multiset) {
            listener.call(&event);
        }
    }
}

void EventBus::registerListener(Listener listener) {
    listeners[listener.type_index].insert(std::move(listener));
}








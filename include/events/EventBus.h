//
// Created by Agamjeet Singh on 17/08/25.
//

#ifndef EVENTBUS_H
#define EVENTBUS_H
#include <queue>
#include <set>
#include <typeindex>
#include <unordered_map>

#include "Event.h"
#include "events/EventExecuteTime.h"

class Listener;

struct ListenerComparator;

class EventBus {
public:
    static EventBus& getInstance();

    void emit(Event event);

    template<typename T>
    void emit(T event, EventExecuteTime execute_time) {
        emit(Event{event, execute_time});
    }

    void executeNow(Event event) const;

    void execute(EventExecuteTime time);

    void registerListener(Listener listener);

private:
    std::unordered_map<EventExecuteTime, std::queue<Event>> events_by_execution;

    std::unordered_map<std::type_index, std::multiset<Listener, ListenerComparator>> listeners;
};



#endif //EVENTBUS_H

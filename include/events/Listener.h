//
// Created by Agamjeet Singh on 17/08/25.
//

#ifndef LISTENER_H
#define LISTENER_H
#include <functional>
#include <typeindex>
#include <utility>

#include "Event.h"
#include "ListenerPriority.h"

class Listener {
public:
    template<typename T>
    explicit Listener(std::function<void(const T&)> cb, ListenerPriority priority = ListenerPriority::NORMAL) :
        priority(priority), type_index(typeid(T)), erased_cb([fn = std::move(cb)](const void* ev) {
            fn(*static_cast<const T*>(ev));
        }) {}

    const ListenerPriority priority;
    const std::type_index type_index;

    void call(const void* ev) const;

private:
    std::function<void(const void*)> erased_cb;
};

struct ListenerComparator {
    bool operator()(const Listener& listenerA, const Listener& listenerB) const {
        return listenerA.priority < listenerB.priority;
    }
};

#endif //LISTENER_H

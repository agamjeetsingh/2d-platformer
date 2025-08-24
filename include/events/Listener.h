//
// Created by Agamjeet Singh on 17/08/25.
//

#ifndef LISTENER_H
#define LISTENER_H
#include <functional>
#include <typeindex>
#include <utility>

#include "Event.h"
#include "EventBus.h"
#include "ListenerPriority.h"

class Listener {
public:
    template<typename T, typename F>
    requires std::invocable<F, const T&> &&
             std::same_as<std::invoke_result_t<F, const T&>, void>
    static Listener make_listener(F&& cb, ListenerPriority priority = ListenerPriority::NORMAL) {
        return Listener(std::type_identity<T>{}, std::forward<F>(cb), priority);
    }

    const ListenerPriority priority;
    const std::type_index type_index;

    void call(const void* ev) const;

private:
    std::function<void(const void*)> erased_cb;

    template<typename T, typename F>
    requires std::invocable<F, const T&> &&
             std::same_as<std::invoke_result_t<F, const T&>, void>
    explicit Listener(std::type_identity<T>, F&& cb, ListenerPriority priority = ListenerPriority::NORMAL) :
        priority(priority), type_index(typeid(T)), erased_cb([fn = std::forward<F>(cb)](const void* ev) {
            const Event& e = *static_cast<const Event*>(ev);
            if (auto payload = e.getIf<T>()) {
                fn(*payload);
            }
        }) {
        EventBus::getInstance().registerListener(*this);
    }
};

struct ListenerComparator {
    bool operator()(const Listener& listenerA, const Listener& listenerB) const {
        return listenerA.priority < listenerB.priority;
    }
};

#endif //LISTENER_H

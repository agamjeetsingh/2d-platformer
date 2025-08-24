//
// Created by Agamjeet Singh on 17/08/25.
//

#ifndef EVENT_H
#define EVENT_H
#include <functional>
#include <typeindex>

#include "EventExecuteTime.h"

class DummyEvent {};

class Event {
public:
    template <typename T>
    explicit Event(T event, EventExecuteTime execute_time) : type_index(typeid(event)), execute_time(execute_time), meta_data(std::make_shared<GeneralEvent<T>>(std::move(event))) {}

    const std::type_index type_index;
    const EventExecuteTime execute_time;

    template <typename EventType>
    [[nodiscard]] const EventType* getIf() const {
        if (type_index != typeid(EventType)) return nullptr;
        auto derived = std::dynamic_pointer_cast<const GeneralEvent<EventType>>(meta_data);
        return derived ? &derived->data : nullptr;
    }

    template <typename EventType>
    [[nodiscard]] bool is() const {
        return type_index == typeid(EventType);
    }

private:
    struct GeneralEventBase {
        virtual ~GeneralEventBase() = default;
    };
    template<typename EventType>
    struct GeneralEvent final : GeneralEventBase {
        GeneralEvent(EventType data) : data(std::move(data)) {}
        EventType data;
    };

    std::shared_ptr<const GeneralEventBase> meta_data;
};

#endif //EVENT_H

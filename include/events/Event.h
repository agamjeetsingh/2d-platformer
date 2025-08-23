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
    explicit Event(T event, EventExecuteTime execute_time);

    const std::type_index type_index;
    const EventExecuteTime execute_time;

    template <typename EventType>
    [[nodiscard]] const EventType* getIf() const;

    template <typename EventType>
    [[nodiscard]] bool is() const;

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

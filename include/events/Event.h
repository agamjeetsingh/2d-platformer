//
// Created by Agamjeet Singh on 17/08/25.
//

#ifndef EVENT_H
#define EVENT_H
#include <functional>
#include <typeindex>

#include "EventExecuteTime.h"


/**
 * @brief A type-erased container for events with execution timing information.
 * 
 * The Event class wraps any event type into a type-erased container while preserving
 * type information through std::type_index. This allows the EventBus to store and
 * dispatch events of different types in a unified manner.
 * 
 * Key Features:
 * - Type erasure with type safety through std::type_index
 * - Type checking with getIf<T>() and is<T>()
 * - Execution time scheduling
 * - Shared ownership of event data
 */
class Event {
public:
    /**
     * @brief Construct an Event from any event type.
     * 
     * @tparam T The concrete event type.
     * @param event The event instance to wrap.
     * @param execute_time When this event should be executed.
     */
    template <typename T>
    explicit Event(T event, EventExecuteTime execute_time) : type_index(typeid(event)), execute_time(execute_time), meta_data(std::make_shared<GeneralEvent<T>>(std::move(event))) {}

    /** @brief The runtime type information of the wrapped event. */
    const std::type_index type_index;
    
    /** @brief When this event should be executed. */
    const EventExecuteTime execute_time;

    /**
     * @brief Safely cast the event to the specified type.
     * 
     * @tparam EventType The type to cast to.
     * @return Pointer to the event data if the cast is valid, nullptr otherwise.
     */
    template <typename EventType>
    [[nodiscard]] const EventType* getIf() const {
        if (type_index != typeid(EventType)) return nullptr;
        auto derived = std::dynamic_pointer_cast<const GeneralEvent<EventType>>(meta_data);
        return derived ? &derived->data : nullptr;
    }

    /**
     * @brief Check if this event is of the specified type.
     * 
     * @tparam EventType The type to check against.
     * @return true if the event is of type EventType, false otherwise.
     */
    template <typename EventType>
    [[nodiscard]] bool is() const {
        return type_index == typeid(EventType);
    }

private:
    /**
     * @brief Base class for type-erased event storage.
     */
    struct GeneralEventBase {
        virtual ~GeneralEventBase() = default;
    };
    
    /**
     * @brief Concrete implementation for storing events of type EventType.
     * @tparam EventType The concrete event type being stored.
     */
    template<typename EventType>
    struct GeneralEvent final : GeneralEventBase {
        /**
         * @brief Construct with event data.
         * @param data The event data to store.
         */
        GeneralEvent(EventType data) : data(std::move(data)) {}
        
        /** @brief The actual event data. */
        EventType data;
    };

    /** @brief Shared pointer to the type-erased event data. */
    std::shared_ptr<const GeneralEventBase> meta_data;
};

#endif //EVENT_H

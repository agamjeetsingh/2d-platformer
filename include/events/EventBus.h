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

/**
 * @brief A singleton event bus that provides communication between different systems.
 * 
 * The EventBus enables communication between different systems (e.g., Input,
 * Physics, Scheduler) through event emission and dispatch. Instead of direct calls,
 * events are emitted and dispatched to listeners at defined execution times.
 * 
 * Key Features:
 * - Type-safe event emission using templates
 * - Priority-based listener execution 
 * - Deferred event execution at specific times
 * - FIFO event processing per execution time
 * 
 * Usage:
 * \code
 * // Emit an event to be executed later
 * EventBus::getInstance().emit(MyEvent{data}, EventExecuteTime::POST_PHYSICS);
 * 
 * // Execute all queued events for a specific time
 * EventBus::getInstance().execute(EventExecuteTime::POST_PHYSICS);
 * \endcode
 * 
 * @note The correctness of the system depends on calling execute() at the appropriate times.
 */
class EventBus {
public:
    /**
     * @brief Get the singleton instance of EventBus.
     * @return Reference to the EventBus singleton instance.
     */
    static EventBus& getInstance();

    /**
     * @brief Emit an event to be executed at the specified time.
     * 
     * Events are wrapped into type-erased Event objects and queued for execution
     * at the specified time. If execute_time is NOW, the event is dispatched immediately.
     * If no listeners are registered for this event type, the event is discarded.
     * 
     * @tparam T The event type (any class can be used as an event).
     * @param event The event instance to emit.
     * @param execute_time When to execute the event (NOW, PRE_INPUT, PRE_PHYSICS, POST_PHYSICS).
     */
    template<typename T>
    void emit(T event, EventExecuteTime execute_time) {
        emit(Event{event, execute_time});
    }

    /**
     * @brief Execute an event immediately, bypassing the queue.
     * @param event The event to execute now.
     */
    void executeNow(Event event) const;

    /**
     * @brief Execute all queued events for the specified execution time.
     * 
     * Events are processed in FIFO order. For each event, listeners are called
     * according to their ListenerPriority: LOWEST → LOW → NORMAL → HIGH → HIGHEST → MONITOR.
     * 
     * @param time The execution time for which to process events.
     */
    void execute(EventExecuteTime time);

    /**
     * @brief Register a listener to receive events.
     * 
     * This method is typically called automatically from the Listener constructor.
     * 
     * @param listener The listener to register.
     */
    void registerListener(Listener listener);

private:
    /** @brief Events queued by execution time. */
    std::unordered_map<EventExecuteTime, std::queue<Event>> events_by_execution;

    /** @brief Listeners organized by event type and sorted by priority. */
    std::unordered_map<std::type_index, std::multiset<Listener, ListenerComparator>> listeners;

    /**
     * @brief Internal method to emit a pre-constructed Event object.
     * @param event The event to emit.
     */
    void emit(Event event);
};



#endif //EVENTBUS_H

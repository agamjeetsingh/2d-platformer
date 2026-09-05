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
#include <mutex>

class Listener;

struct ListenerComparator;

/**
 * @brief An event bus that provides communication between different systems.
 *
 * An EventBus enables communication between different systems through event emission
 * and dispatch, instead of direct calls between them. An application is expected to
 * own as many EventBus instances as it needs (e.g. one per phase of its own update
 * loop) and to call execute() on each at the point in that loop where its queued
 * events should be dispatched.
 *
 * Key Features:
 * - Type-safe event emission using templates
 * - Priority-based listener execution
 * - Either deferred (queued) or immediate event execution
 * - FIFO event processing
 *
 * Usage:
 * \code
 * // Emit an event to be executed on the next execute() call
 * bus.emit(MyEvent{data});
 *
 * // Emit an event to be dispatched immediately, bypassing the queue
 * bus.emitNow(MyEvent{data});
 *
 * // Execute all queued events
 * bus.execute();
 * \endcode
 *
 * @note The correctness of the system depends on calling execute() at the appropriate times.
 */
class EventBus {
public:
    /**
     * @brief Emit an event to be executed on the next execute() call.
     *
     * Events are wrapped into type-erased Event objects and queued for execution.
     * If no listeners are registered for this event type, the event is discarded.
     *
     * @tparam T The event type (any class can be used as an event).
     * @param event The event instance to emit.
     */
    template<typename T>
    void emit(T event) {
        emit(Event{event});
    }

    /**
     * @brief Emit an event and dispatch it immediately, bypassing the queue.
     *
     * @tparam T The event type (any class can be used as an event).
     * @param event The event instance to emit.
     */
    template<typename T>
    void emitNow(T event) {
        executeNow(Event{event});
    }

    /**
     * @brief Execute all queued events.
     *
     * Events are processed in FIFO order. For each event, listeners are called
     * according to their ListenerPriority: LOWEST → LOW → NORMAL → HIGH → HIGHEST → MONITOR.
     *
     * @note Acquires event and listener locks. But doesn't hold any locks when a listener is called.
     */
    void execute();

    /**
     * @brief Register a listener to receive events.
     *
     * This method is typically called automatically from the Listener constructor.
     *
     * @param listener The listener to register.
     */
    void registerListener(Listener listener);

    /**
     * @brief Clears all events in the queue and unregisters all listeners.
     * @warning This function should NOT be called from inside a listener as it will lead to a deadlock.
     */
    void clear();

private:
    /** @brief Queued events, to be processed on the next execute() call. */
    std::queue<Event> events;

    /** @brief Listeners organized by event type and sorted by priority. */
    std::unordered_map<std::type_index, std::multiset<Listener, ListenerComparator>> listeners;

    std::mutex events_mtx;
    std::mutex listeners_mtx;

    /**
     * @brief Internal method to emit a pre-constructed Event object.
     * @param event The event to emit.
     */
    void emit(Event event);

    /**
     * @brief Execute an event immediately, bypassing the queue.
     * @param event The event to execute now.
     */
    void executeNow(Event event);
};



#endif //EVENTBUS_H

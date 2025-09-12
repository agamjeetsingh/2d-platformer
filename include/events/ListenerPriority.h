//
// Created by Agamjeet Singh on 17/08/25.
//

#ifndef EVENTPRIORITY_H
#define EVENTPRIORITY_H


/**
 * @brief Defines the execution priority of event listeners.
 * 
 * Listeners are called in the following order during event dispatch:
 * LOWEST → LOW → NORMAL → HIGH → HIGHEST → MONITOR
 * 
 * Priority Guidelines:
 * - \b LOWEST: System initialization, early setup
 * - \b LOW: General background processing
 * - \b NORMAL: Default priority for most listeners
 * - \b HIGH: Important game logic, player actions
 * - \b HIGHEST: Decides the fate of the event
 * - \b MONITOR: Logging, debugging, analytics (should not modify game state)
 * 
 * @note If multiple listeners have the same priority, their relative execution order is not guaranteed.
 */
enum class ListenerPriority {
    LOWEST,  ///< Execute first - system initialization
    LOW,     ///< Low priority background tasks
    NORMAL,  ///< Default priority for most listeners
    HIGH,    ///< High priority game logic
    HIGHEST, ///< Decides the fate of the event
    MONITOR  ///< Execute last - monitoring and logging only
};



#endif //EVENTPRIORITY_H

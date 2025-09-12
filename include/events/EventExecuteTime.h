//
// Created by Agamjeet Singh on 17/08/25.
//

#ifndef EVENTEXECUTETIME_H
#define EVENTEXECUTETIME_H



/**
 * @brief Defines when events should be executed during the game loop.
 * 
 * Events can be scheduled to execute at specific times in the game loop,
 * allowing for proper ordering of operations and avoiding conflicts between
 * different systems.
 * 
 * Execution Flow:
 * 1. \b PRE_INPUT: Before input processing
 * 2. \b PRE_PHYSICS: Before physics simulation
 * 3. \b POST_PHYSICS: After physics simulation
 * 4. \b NOW: Immediate execution (bypasses queue)
 * 
 * @note Events with the same execution time are processed in FIFO order.
 */
enum class EventExecuteTime {
    NOW,         ///< Execute immediately, bypassing the event queue
    PRE_INPUT,   ///< Execute before input processing
    PRE_PHYSICS, ///< Execute before physics simulation
    POST_PHYSICS ///< Execute after physics simulation
};



#endif //EVENTEXECUTETIME_H

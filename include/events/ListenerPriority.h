//
// Created by Agamjeet Singh on 17/08/25.
//

#ifndef EVENTPRIORITY_H
#define EVENTPRIORITY_H


/**
 * Listeners are called in following order: \code LOWEST\endcode -> \code LOW\endcode -> \code NORMAL\endcode ->
 * \code HIGH\endcode -> \code HIGHEST\endcode -> \code MONITOR\endcode.
 */
enum class ListenerPriority {
    LOWEST,
    LOW,
    NORMAL,
    HIGH,
    HIGHEST,
    MONITOR
};



#endif //EVENTPRIORITY_H

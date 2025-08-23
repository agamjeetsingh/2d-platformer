//
// Created by Agamjeet Singh on 17/08/25.
//

#include "../../include/events/Event.h"

template<typename T>
Event::Event(T event, EventExecuteTime execute_time): type_index(typeid(event)), execute_time(execute_time), meta_data(std::make_shared<GeneralEvent<T>>(std::move(event))) {}

template<typename EventType>
const EventType *Event::getIf() const {
    if (type_index != typeid(EventType)) return nullptr;
    auto ptr = dynamic_cast<GeneralEvent<EventType>*>(&meta_data);
    return ptr ? &ptr->data : nullptr;
}

template<typename EventType>
bool Event::is() const {
    return type_index == typeid(EventType);
}


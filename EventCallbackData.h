#pragma once

#include <functional>
#include "ProcessEvent.h"

struct EventCallbackData
{

    using EventCallback = std::function<void(ProcessEvent, void*)>;

    EventCallback callback;
    void* context;
    int id;
};

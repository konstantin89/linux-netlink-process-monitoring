#pragma once

#include <functional>
#include "ProcessEvent.h"

struct EventCallbackData
{
    using EventCallback = std::function<void(ProcessEvent, void*)>;

    EventCallbackData(EventCallback aCallback, void* aContext, uint64_t aId)
    {
        callback = aCallback;
        context = aContext;
        id = aId;
    }

    ~EventCallbackData()
    {
        /* EMPTY */
    }

    EventCallback callback;
    void* context;
    uint64_t id;
};

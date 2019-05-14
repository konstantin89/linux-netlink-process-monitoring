#pragma once

#include <vector>
#include <functional>
#include <thread>

#include "utils/ThreadSafeList.h"
#include "ProcessEvent.h"
#include "EventCallbackData.h"

class ProcessMonitor
{
public:

    ProcessMonitor();
    ~ProcessMonitor();

    int Connect();

    /**
     * @returns - Callback ID.
     */
    int AddCallback(EventCallbackData aEventCallback);

    int Start();




private:

    int subscribeToProcEvents();

    int unSubscribeToProcEvents();

    ThreadSafeList<ProcessEvent> mProcEventsList;

    std::vector<EventCallbackData> mEventCallbacks;

    std::thread mEventProducerThread;

private:

    static void netlinkClient(ProcessMonitor& aProcessMonitor);

};
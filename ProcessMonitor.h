#pragma once

#include <vector>
#include <functional>
#include <thread>

#include "utils/ThreadSafeList.h"
#include "ProcessEvent.h"
#include "EventCallbackData.h"
#include "IdGenerator.h"

#define INVALID_SOCKET -1
#define SUCCESS_CODE 0
#define ERROR_CODE -1

class ProcessMonitor
{
public:

    using EventCallback = EventCallbackData::EventCallback;

    ProcessMonitor();
    ~ProcessMonitor();

    int Connect();

    /**
     * @returns - Callback ID.
     */
    uint64_t AddCallback(EventCallback aEventCallback, void* context);

    int RemoveCallback(uint64_t aCallbackId);

    int Start();

    int Stop();


private:

    int connectToNetlinkSocket();

    int disconnectFromNetlinkSocket();

    int subscribeToProcEvents();

    int unSubscribeFromProcEvents();

    int toggleSubscriptionStatus(bool aEnable);

    bool terminationCommandReceived();

private:

    int mNetlinkSock;

    bool mGotTermCommand;

    ThreadSafeList<ProcessEvent> mProcEventsList;

    std::vector<EventCallbackData> mEventCallbacks;

    std::thread mEventProducerThread; 

    std::thread mCallbackRunnerThread;

    IdGenerator mCallbackIdGenerator;

private:

    static void netlinkClient(ProcessMonitor* aProcessMonitor);

    static void callbackRunner(ProcessMonitor* aProcessMonitor);

};
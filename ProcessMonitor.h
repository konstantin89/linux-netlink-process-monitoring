#pragma once

#include <vector>
#include <functional>
#include <thread>

#include "utils/ThreadSafeList.h"
#include "ProcessEvent.h"
#include "EventCallbackData.h"

#define INVALID_SOCKET -1
#define SUCCESS_CODE 0
#define ERROR_CODE -1

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

    int RemoveCallback(int aCallbackId);

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

private:

    static void netlinkClient(ProcessMonitor* aProcessMonitor);

    static void callbackRunner(ProcessMonitor* aProcessMonitor);

};
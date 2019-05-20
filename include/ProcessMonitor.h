#pragma once

#include <functional>
#include <thread>

#include "utils/ThreadSafeList.h"
#include "ProcessEvent.h"
#include "EventCallbackData.h"
#include "IdGenerator.h"

#define INVALID_SOCKET -1
#define SUCCESS_CODE EXIT_SUCCESS
#define ERROR_CODE EXIT_FAILURE

class ProcessMonitor
{
public:

    using EventCallback = EventCallbackData::EventCallback;

    ProcessMonitor();
    ~ProcessMonitor();

    /**
     * @brief: Establish netlink connection. Must be called before Start.
     * 
     * @param: void.
     * 
     * @returns: SUCCESS_CODE, ERROR_CODE.
     */
    int Connect();

    /**
     * @brief: Add callback that will be called on each process event.
     * 
     * @param: aEventCallback - function that will be called.
     * @param: aContext - Context that aEventCallback will be able to access.
     * 
     * @returns: Callback ID. This ID can be used to remove this callback.
     */
    uint64_t AddCallback(EventCallback aEventCallback, void* aContext);

    /**
     * @brief: Remove previously registered callback.
     * 
     * @param: aCallbackId - Callback ID returned by AddCallback method.
     * 
     * @returns: void
     */
    void RemoveCallback(uint64_t aCallbackId);

    /**
     * @brief: Start process events monitoring. Must be called after Connect.
     * 
     * @param: void.
     * 
     * @returns: SUCCESS_CODE, ERROR_CODE
     */
    int Start();

    /** 
     * @brief: Stops process event monitoring.
     * 
     * @param: void.
     * 
     * @returns: SUCCESS_CODE, ERROR_CODE
     */
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

    ThreadSafeList<EventCallbackData> mEventCallbacks;

    std::thread mEventProducerThread; 

    std::thread mCallbackRunnerThread;

    IdGenerator mCallbackIdGenerator;

private:

    static void netlinkClient(ProcessMonitor* aProcessMonitor);

    static void callbackRunner(ProcessMonitor* aProcessMonitor);

};
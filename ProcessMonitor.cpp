#include "ProcessMonitor.h"

#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/connector.h>
#include <linux/cn_proc.h>
#include <unistd.h>
#include <string.h>


ProcessMonitor::ProcessMonitor()
{
    mNetlinkSock = INVALID_SOCKET;
    mGotTermCommand = false;
}

ProcessMonitor::~ProcessMonitor()
{
    Stop();
}

int ProcessMonitor::Connect()
{
    if(connectToNetlinkSocket() != SUCCESS_CODE)
    {
        return ERROR_CODE;
    }

    if(subscribeToProcEvents() != SUCCESS_CODE)
    {
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}

int ProcessMonitor::Start()
{
    mGotTermCommand = false;

    mEventProducerThread = std::thread(netlinkClient, this);
    mCallbackRunnerThread = std::thread(callbackRunner, this);

    return SUCCESS_CODE;
}

int ProcessMonitor::Stop()
{
    mGotTermCommand = true;
    unSubscribeFromProcEvents();
    disconnectFromNetlinkSocket();
    
    if(mEventProducerThread.joinable())
    {
        mEventProducerThread.join();
    }

    if(mCallbackRunnerThread.joinable())
    {
        mCallbackRunnerThread.join();
    }
    
    return SUCCESS_CODE;
}



int ProcessMonitor::connectToNetlinkSocket()
{

    if(mNetlinkSock != INVALID_SOCKET)
    {
        return SUCCESS_CODE;
    }

    int lRetVal;
    struct sockaddr_nl sa_nl;

    mNetlinkSock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
    if (mNetlinkSock == INVALID_SOCKET) {
        perror("socket");
        return ERROR_CODE;
    }

    sa_nl.nl_family = AF_NETLINK;
    sa_nl.nl_groups = CN_IDX_PROC;
    sa_nl.nl_pid = getpid();

    lRetVal = bind(mNetlinkSock, (struct sockaddr *)&sa_nl, sizeof(sa_nl));
    if (lRetVal == ERROR_CODE) {
        perror("bind");
        disconnectFromNetlinkSocket();
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}

int ProcessMonitor::disconnectFromNetlinkSocket()
{
    close(mNetlinkSock);
    mNetlinkSock = INVALID_SOCKET;
    return SUCCESS_CODE;
}

int ProcessMonitor::subscribeToProcEvents()
{
    return toggleSubscriptionStatus(true);
}

int ProcessMonitor::unSubscribeFromProcEvents()
{
    return toggleSubscriptionStatus(false);
}

int ProcessMonitor::toggleSubscriptionStatus(bool aEnable)
{

    if(mNetlinkSock == INVALID_SOCKET)
    {
        return ERROR_CODE;
    }

    int lRetVal;
    struct __attribute__ ((aligned(NLMSG_ALIGNTO))) {
        struct nlmsghdr nl_hdr;
        struct __attribute__ ((__packed__)) {
            struct cn_msg cn_msg;
            enum proc_cn_mcast_op cn_mcast;
        };
    } nlcn_msg;

    memset(&nlcn_msg, 0, sizeof(nlcn_msg));
    nlcn_msg.nl_hdr.nlmsg_len = sizeof(nlcn_msg);
    nlcn_msg.nl_hdr.nlmsg_pid = getpid();
    nlcn_msg.nl_hdr.nlmsg_type = NLMSG_DONE;

    nlcn_msg.cn_msg.id.idx = CN_IDX_PROC;
    nlcn_msg.cn_msg.id.val = CN_VAL_PROC;
    nlcn_msg.cn_msg.len = sizeof(enum proc_cn_mcast_op);

    nlcn_msg.cn_mcast = aEnable ? PROC_CN_MCAST_LISTEN : PROC_CN_MCAST_IGNORE;

    lRetVal = send(mNetlinkSock, &nlcn_msg, sizeof(nlcn_msg), 0);
    if (lRetVal == ERROR_CODE) {
        perror("netlink send");
        return ERROR_CODE;
    }

    return SUCCESS_CODE;
}

bool ProcessMonitor::terminationCommandReceived()
{
    return mGotTermCommand;
}

#include <iostream>

void ProcessMonitor::netlinkClient(ProcessMonitor* aProcessMonitor)
{

    int rc;
    struct __attribute__ ((aligned(NLMSG_ALIGNTO))) {
        struct nlmsghdr nl_hdr;
        struct __attribute__ ((__packed__)) {
            struct cn_msg cn_msg;
            struct proc_event proc_ev;
        };
    } nlcn_msg;

    while (aProcessMonitor->terminationCommandReceived() == false) {
        
        rc = recv(aProcessMonitor->mNetlinkSock, &nlcn_msg, sizeof(nlcn_msg), 0);

        auto lEventArrivalTime = std::chrono::system_clock::now();

        if (rc == 0) {
            /* shutdown? */
            return;
        } else if (rc == -1) {
            if (errno == EINTR) continue;
            perror("netlink recv");
            return;
        }

        ProcessEvent lProcEvent;
        lProcEvent.arrivalTime = lEventArrivalTime.time_since_epoch().count(); 
        lProcEvent.eventData = nlcn_msg.proc_ev;

        aProcessMonitor->mProcEventsList.PushBack(lProcEvent);

        std::cout << "Pushed Event" << std::endl;
    }

    std::cout << "[ ] Netlink client thread is terminating" << std::endl;
 }

void ProcessMonitor::callbackRunner(ProcessMonitor* aProcessMonitor)
{

    ProcessEvent lProcEvent;

    while (aProcessMonitor->terminationCommandReceived() == false) {
        
        aProcessMonitor->mProcEventsList.WaitAndPopFront(lProcEvent);

        std::cout << "Poped event" << std::endl;

        for(auto callbackData : aProcessMonitor->mEventCallbacks)
        {
            callbackData.callback(lProcEvent, callbackData.context);
        }
       
    }

    std::cout << "[ ] callbackRunner thread is terminating" << std::endl;
 }

 
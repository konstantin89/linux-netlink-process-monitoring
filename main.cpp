
#include <iostream>

#include "ProcessMonitor.h"

#define UNUSED_PARAMETER(x) (void)(x)

void logCallbackExample(ProcessEvent aEvent, void* aContext)
{
    UNUSED_PARAMETER(aContext);

     switch (aEvent.eventData.what) {
            case proc_event::PROC_EVENT_NONE:
                //printf("Set mcast listen ok \n");
                break;

            case proc_event::PROC_EVENT_FORK:
                printf("fork: parent tid=%d pid=%d, child tid=%d pid=%d, timestamp=%lu \n",
                        aEvent.eventData.event_data.fork.parent_pid,
                        aEvent.eventData.event_data.fork.parent_tgid,
                        aEvent.eventData.event_data.fork.child_pid,
                        aEvent.eventData.event_data.fork.child_tgid,
                        aEvent.GetTimeStamp());
                break;

            case proc_event::PROC_EVENT_EXEC:
                printf("exec: tid=%d pid=%d, timestamp=%lu\n",
                        aEvent.eventData.event_data.exec.process_pid,
                        aEvent.eventData.event_data.exec.process_tgid,
                        aEvent.GetTimeStamp());
                break;

            case proc_event::PROC_EVENT_UID:
                printf("uid change: tid=%d pid=%d from %d to %d, timestamp=%lu \n",
                        aEvent.eventData.event_data.id.process_pid,
                        aEvent.eventData.event_data.id.process_tgid,
                        aEvent.eventData.event_data.id.r.ruid,
                        aEvent.eventData.event_data.id.e.euid,
                        aEvent.GetTimeStamp());
                break;

            case proc_event::PROC_EVENT_GID:
                printf("gid change: tid=%d pid=%d from %d to %d, timestamp=%lu \n",
                        aEvent.eventData.event_data.id.process_pid,
                        aEvent.eventData.event_data.id.process_tgid,
                        aEvent.eventData.event_data.id.r.rgid,
                        aEvent.eventData.event_data.id.e.egid,
                        aEvent.GetTimeStamp());
                break;

            case proc_event::PROC_EVENT_EXIT:
                printf("exit: tid=%d pid=%d exit_code=%d, timestamp=%lu \n",
                        aEvent.eventData.event_data.exit.process_pid,
                        aEvent.eventData.event_data.exit.process_tgid,
                        aEvent.eventData.event_data.exit.exit_code,
                        aEvent.GetTimeStamp());
                break;
                
            default:
                //printf("Not handled event \n");
                break;
        }
}

int main()
{
    ProcessMonitor lProcMon;

    auto lCallbackId = lProcMon.AddCallback(logCallbackExample, NULL);

    if(lProcMon.Connect() != SUCCESS_CODE)
    {
        std::cout << "[X] ProcessMonitor::Connect failed (make sure to use sudo)" << std::endl;
        return 1;
    }

    if(lProcMon.Start() != SUCCESS_CODE)
    {
        std::cout << "[X] ProcessMonitor::Start failed" << std::endl;
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(10));

    lProcMon.RemoveCallback(lCallbackId);
    
    if(lProcMon.Stop() != SUCCESS_CODE)
    {
        std::cout << "[X] ProcessMonitor::Stop failed" << std::endl;
        return 1;
    }

}

#include <iostream>

#include "ProcessMonitor.h"

int main()
{
    std::cout << "[ ] starting process monitor" << std::endl;

    ProcessMonitor lProcMon;

    if(lProcMon.Connect() != SUCCESS_CODE)
    {
        std::cout << "[X] ProcessMonitor::Connect failed" << std::endl;
        return 1;
    }

    if(lProcMon.Start() != SUCCESS_CODE)
    {
        std::cout << "[X] ProcessMonitor::Start failed" << std::endl;
        return 1;
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if(lProcMon.Stop() != SUCCESS_CODE)
    {
        std::cout << "[X] ProcessMonitor::Stop failed" << std::endl;
        return 1;
    }

}
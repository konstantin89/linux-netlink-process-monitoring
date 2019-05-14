#pragma once

#include <chrono>
#include <linux/cn_proc.h>

class ProcessEvent
{
public:

    struct proc_event eventData;
    std::chrono::microseconds arrivalTime;

private:

};
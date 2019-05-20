#pragma once

#include <linux/cn_proc.h>

class ProcessEvent
{
public:

    struct proc_event eventData;
   
   uint64_t GetTimeStamp()
   {
       return eventData.timestamp_ns;
   }

};
# linux-netlink-process-monitoring

## Brief
Linux process monitor implemented using [Netlink](http://man7.org/linux/man-pages/man7/netlink.7.html) mechanism.</br> 
This monitor allows registration of callbacks on Linux process events (exec, forc etc.).</br> 


## Permissions

Since only supoer users can open netlinc communication ports, sudo have </br>
to be used when running any process monitoring processes.


## Build

This project is using Cmake to generate make files.</br> 
To generate project binaries execute the following commands </br>
while starting from CMakeLists.txt directory:  </br>
* mkdir build 
* cd build 
* cmake ..
* make



## Binaries

### lib_process_monitor

Static library containig all process monitoring functionality. </br>

### monitor_example

Executable that uses lib_process_monitor to log process events. </br>
This executable registers logger callback to the process events monitor</br>
and prints events to stdout.

## TODOs

* Callback context should be shared_ptr, and be released when callback is deleted.
* Limit max number of messages in queue.

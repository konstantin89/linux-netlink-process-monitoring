cmake_minimum_required (VERSION 2.6)

project (linux-netlink-process-monitor)

set (CXX_FLAGS "-std=c++14 -Wall -Wextra -g")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

add_library(
    lib_process_monitor
    ProcessMonitor.cpp
)

add_executable(
    monitor_example
    main.cpp)

TARGET_LINK_LIBRARIES(
    lib_process_monitor
    pthread
)

TARGET_LINK_LIBRARIES(
    monitor_example
    lib_process_monitor
)

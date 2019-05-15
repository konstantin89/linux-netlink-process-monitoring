cmake_minimum_required (VERSION 2.6)

project (linux-netlink-process-monitor)

set (DEBUG_FLAGS "-g")
set (CXX_FLAGS "-std=c++14 -Wall -Wextra")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

if(${CMAKE_BUILD_TYPE} EQUAL Debug)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEBUG_FLAGS}")
endif()

add_executable(
    monitor 
    ProcessMonitor.cpp
    main.cpp)

TARGET_LINK_LIBRARIES(
    monitor
    pthread
)

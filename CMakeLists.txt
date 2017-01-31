cmake_minimum_required(VERSION 3.6)
project(RealTimeGarbageCollector)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp gcPtr.cpp gcPtr.h garbageCollector.cpp garbageCollector.h heapItem.cpp heapItem.h encoding.cpp encoding.h allocator/addressRange.cpp allocator/addressRange.h allocator/memoryChunk.cpp allocator/memoryChunk.h)
add_executable(RealTimeGarbageCollector ${SOURCE_FILES})
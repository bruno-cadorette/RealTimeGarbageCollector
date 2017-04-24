cmake_minimum_required(VERSION 3.6)
project(RealTimeGarbageCollector)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ftemplate-depth=1200")

set(SOURCE_FILES main.cpp sha256.h sha256.cpp Gc.h tests.h tests.cpp garbageCollector.cpp garbageCollector.h GcStats.cpp GcStats.h heapItem.cpp heapItem.h encoding.cpp encoding.h allocator/addressRange.cpp allocator/addressRange.h allocator/memoryChunk.cpp allocator/memoryChunk.h allocator/gcAllocator.cpp allocator/gcAllocator.h allocator/gcIndex.cpp allocator/gcIndex.h allocator/freeList.cpp allocator/freeList.h dependentArray.h allocator/objectSize.h)
add_executable(RealTimeGarbageCollector ${SOURCE_FILES})

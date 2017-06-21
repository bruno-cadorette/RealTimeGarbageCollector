//
// Created by bruno on 07/02/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_FREELIST_H
#define REALTIMEGARBAGECOLLECTOR_FREELIST_H


#include <cstddef>
#include <vector>
#include <set>
#include <functional>
#include <boost/icl/interval_set.hpp>
#include "memoryChunk.h"


class freeList {
    using memoryInterval = boost::icl::interval<char*>;
    std::size_t objSize;
    boost::icl::interval_set<char*> freeMemory;
    void addMemory(char* ptr, std::size_t size);
public:
    freeList(std::size_t objSize) : objSize{objSize}{

    }
    void* allocate();
    void free(void* ptr);
    void addMemory(memoryChunkHeader& chunk);
    bool canAllocate();
};

#endif //REALTIMEGARBAGECOLLECTOR_FREELIST_H

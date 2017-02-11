//
// Created by bruno on 31/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_ALLOCATOR_H
#define REALTIMEGARBAGECOLLECTOR_ALLOCATOR_H


#include <vector>
#include <algorithm>
#include "memoryChunk.h"
#include "addressRange.h"
#include "gcIndex.h"
#include "freeList.h"

class allocator {
    gcTopIndex gcIndex;
    std::vector<freeList> freeLists;
    std::vector<memoryChunk> chunks;
    addressRange range;
    freeList& getFreeList(std::size_t i);

public:
    void* allocate(std::size_t alloc_size);
};


#endif //REALTIMEGARBAGECOLLECTOR_ALLOCATOR_H

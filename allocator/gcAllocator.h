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
#include "../dependentArray.h"



using freeListArray = std::vector<freeList>;

class gcAllocator {
    gcTopIndex gcIndex;
    freeListArray freeLists;
    addressRange range;
    freeList& getFreeList(std::size_t i);
    bool isSmallObject(std::size_t size);
    void* allocateSmall(std::size_t size);
    void* allocateBig(std::size_t size);

public:
    gcAllocator();
    bool isValidPtr(char* ptr);
    void* allocate(std::size_t alloc_size);
    template<class T>
    T* allocate(){
        auto lst = getFreeList(sizeof(T));
        if(!lst.canAllocate()){
            auto m = new memoryChunkHeaderImpl<sizeof(T)>;
            auto ptr = reinterpret_cast<std::size_t>(m->startOfMemory());
            gcIndex.getOrSetData(ptr)->getData(ptr) = m;
            lst.addMemory(*m);
        }
        return static_cast<T*>(range.newPrt(lst.allocate(), sizeof(T)));
    }
};


#endif //REALTIMEGARBAGECOLLECTOR_ALLOCATOR_H

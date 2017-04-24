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

    template<class F>
    void iterateInMemory(F f)
    {
        for(auto& mid : gcIndex.rawData())
        {
            if(mid){
                for(auto& bottom : mid->rawData())
                {
                    if(bottom)
                    {
                        f(bottom);
                    }
                }
            }
        }
    }

public:
    gcAllocator();
    bool isValidPtr(char* ptr);
    void mark(char* ptr){
        auto i = reinterpret_cast<std::size_t>(ptr);
        gcIndex.getOrSetData(i)->getData(i)->mark(ptr);
    }
    bool isMarked(char* ptr) {
        auto i = reinterpret_cast<std::size_t>(ptr);
        return gcIndex.getOrSetData(i)->getData(i)->isMarked(ptr);
    }
    void collectMemory(){
        iterateInMemory([](memoryChunkHeader* chunk){
           if(chunk->canBeDeleted()){
               delete chunk;
               chunk = nullptr;
           }
           else{
               chunk->unMarks();
           }
        });
    }

    void* allocate(std::size_t alloc_size);
    std::vector<char*> innersPtr(char* ptr);
    template<class T>
    void* allocate(){
        auto lst = getFreeList(sizeof(T));
        if(!lst.canAllocate()){
            auto m = new memoryChunkHeaderImpl<sizeof(T)>;
            auto ptr = reinterpret_cast<std::size_t>(m->startOfMemory());
            gcIndex.getOrSetData(ptr)->getData(ptr) = m;
            lst.addMemory(*m);
        }
        return range.newPrt(lst.allocate(), sizeof(T));
    }
};


#endif //REALTIMEGARBAGECOLLECTOR_ALLOCATOR_H

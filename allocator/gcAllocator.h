//
// Created by bruno on 31/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_ALLOCATOR_H
#define REALTIMEGARBAGECOLLECTOR_ALLOCATOR_H


#include <vector>
#include <algorithm>
#include <cstddef>
#include "memoryChunk.h"
#include "addressRange.h"
#include "gcIndex.h"
#include "freeList.h"
#include "../dependentArray.h"


using freeListArray = std::vector<freeList>;

class gcAllocator {
    const std::size_t COLLECT_EVERY_N_FULL_FREELISTS = 2;

    gcTopIndex gcIndex;
    freeListArray freeLists;
    addressRange range;
    freeList& getFreeList(std::size_t i);
    std::size_t fullFreeListsCount = 1;

    template<class F>
    void iterateInMemory(F f) {
        for(auto& mid : gcIndex.rawData()) {
            if(mid) {
                std::cout << "Hi." << std::endl;
                for (std::size_t i = 0; i < mid->rawData().size(); ++i) {
                    if (mid->rawData()[i]) {
                        std::cout << "[" << i << "] = " << mid->rawData()[i] << std::endl;
                        f(mid->rawData()[i]);
                        std::cout << "AFTER: " << mid->rawData()[i] << std::endl;
                    }
                }
                /* for(auto& bottom : mid->rawData()) { */
                /*     if(bottom) { */
                /*         std::cout << bottom << std::endl; */
                /*         f(bottom); */
                /*     } */
                /* } */
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
    void collectMemory() {
        iterateInMemory([](memoryChunkHeader*& chunk){
           if (chunk->canBeDeleted()) {
               delete chunk;
               chunk = nullptr;
               std::cout << "Set to null." << std::endl;
           }
           else {
               chunk->unMarks();
           }
        });
    }

    void* allocate(std::size_t alloc_size);
    std::vector<char*> innersPtr(char* ptr);
    template<class T, class F>
    void* allocate(F collect) {
        auto lst = getFreeList(sizeof(T));

        if (!lst.canAllocate()) {
            ++fullFreeListsCount;

            // +1 so that we don't collect on the first one
            if ((fullFreeListsCount + 1) % COLLECT_EVERY_N_FULL_FREELISTS == 0) {
                collect();
                return allocate<T>(collect); // will finish because we incremented the count
            }

            auto m = new memoryChunkHeaderImpl<sizeof(T)>;
            auto ptr = reinterpret_cast<std::size_t>(m->startOfMemory());
            gcIndex.getOrSetData(ptr)->getData(ptr) = m;
            lst.addMemory(*m);
        }
        return range.newPrt(lst.allocate(), sizeof(T));
    }
};


#endif //REALTIMEGARBAGECOLLECTOR_ALLOCATOR_H

//
// Created by bruno on 07/02/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_FREELIST_H
#define REALTIMEGARBAGECOLLECTOR_FREELIST_H


#include <cstddef>
#include <vector>
#include <forward_list>
#include "memoryChunk.h"


class freeList {
    std::size_t objSize;
    struct freeListItem {
        char* startOfAllocable;
        std::size_t allocableSize;
        freeListItem(char* startOfAllocable, std::size_t size)
                : startOfAllocable{startOfAllocable}, allocableSize{size}{

        }
        std::pair<void*, bool> allocate(std::size_t objSize){
            if(objSize == allocableSize){
                auto ptr = startOfAllocable;
                allocableSize -= objSize;
                startOfAllocable += objSize;
                return std::make_pair(ptr, false);
            }
            else{
                return std::make_pair(startOfAllocable, true);
            }
        }
        bool canMergePtr(void* ptr) const noexcept {
            return startOfAllocable + allocableSize == ptr;
        }
        void addPtr(std::size_t objSize) noexcept {
            allocableSize += objSize;
        };
    };

    std::forward_list<freeListItem> freeMemory;
    std::size_t getIndex(std::size_t i);
    void addMemory(void* ptr, std::size_t size);
public:
    freeList(std::size_t objSize) : objSize{objSize}{

    }
    void* allocate();
    void addMemory(memoryChunk& chunk);
    void free(void* ptr);
    bool canAllocate();
};


#endif //REALTIMEGARBAGECOLLECTOR_FREELIST_H

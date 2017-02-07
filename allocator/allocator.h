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

class allocator {
    gcTopIndex gcIndex;
    std::vector<memoryChunk*> memory;
    addressRange range;

public:
    void* allocate(std::size_t alloc_size){
        void* ptr;
        auto it = std::find_if(memory.begin(), memory.end(), [&](memoryChunk* chunk){
           return chunk->getSize() == alloc_size;
        });
        if (it == memory.end()){
            auto c = new memoryChunk(alloc_size);
            memory.push_back(c);
            ptr = c->allocate();
        }
        else{
            ptr = (*it)->allocate();
        }
        range.newPrt((std::size_t)ptr);
        return ptr;
    }
};


#endif //REALTIMEGARBAGECOLLECTOR_ALLOCATOR_H

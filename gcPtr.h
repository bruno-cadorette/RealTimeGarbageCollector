//
// Created by bruno on 21/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_GCPTR_H
#define REALTIMEGARBAGECOLLECTOR_GCPTR_H

#include "garbageCollector.h"

template<class T, int size = 0>
class gcPtr {

public:
    uint64_t ptrIndex;
    gcPtr(T* ptr = nullptr){
        ptrIndex = garbageCollector::get().allocate(ptr);
    }
    ~gcPtr(){
        garbageCollector::get().removeReference(ptrIndex);
    }
};


#endif //REALTIMEGARBAGECOLLECTOR_GCPTR_H

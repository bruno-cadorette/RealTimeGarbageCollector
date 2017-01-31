//
// Created by bruno on 21/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_GCPTR_H
#define REALTIMEGARBAGECOLLECTOR_GCPTR_H

#include "garbageCollector.h"

template<class T, int size = 0>
class gcPtr {
    encodedPtr ptrIndex;
public:
    gcPtr(T* a = nullptr){
        ptrIndex = garbageCollector::get().allocate(a);
    }
    ~gcPtr(){
        garbageCollector::get().removeReference(ptrIndex);
    }
    T* operator->() {
        return (T*)garbageCollector::get()[ptrIndex];
    }
};


#endif //REALTIMEGARBAGECOLLECTOR_GCPTR_H

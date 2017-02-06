//
// Created by bruno on 06/02/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_GCINDEX_H
#define REALTIMEGARBAGECOLLECTOR_GCINDEX_H


#include "memoryChunk.h"

const int HighBitsSize = 10;
const int MiddleBitsSize = 10;
const int LowBitsSize = 12;
const int IndexSize = 1024;

template<class T, unsigned int BitLocation>
class gcIndex {
    T* index[IndexSize] = {};
    u_int16_t getIndexFromPtr(u_int32_t ptr) const noexcept{
        return (ptr >> BitLocation) & (IndexSize - 1);
    }

public:
    T* getData(uint32_t ptr) const{
        return index[getIndexFromPtr(ptr)];
    }
};

using gcBottomIndex = gcIndex<memoryChunk, LowBitsSize>;
using gcTopIndex = gcIndex<gcBottomIndex, LowBitsSize + MiddleBitsSize>;



#endif //REALTIMEGARBAGECOLLECTOR_GCINDEX_H

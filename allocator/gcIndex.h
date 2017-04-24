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



template<class T, std::size_t BitLocation>
class gcIndex {
    std::array<T*, IndexSize> data;
public:
    gcIndex() : data{} {}
    std::array<T*, IndexSize>& rawData(){
        return data;
    }
    std::size_t getIndexFromPtr(std::size_t ptr) const noexcept{
        return (ptr >> BitLocation) & (IndexSize - 1);
    }
    T*& getData(u_int64_t ptr) {
        return data[getIndexFromPtr(ptr)];
    }
    T* getOrSetData(u_int64_t  ptr) {
        auto i = getIndexFromPtr(ptr);
        auto& a = data[i];
        if (!a) {
            a = new T;
        }
        return a;
    }
};


using gcBottomIndex = gcIndex<memoryChunkHeader, LowBitsSize>;

using gcTopIndex = gcIndex<gcBottomIndex, LowBitsSize + MiddleBitsSize>;



#endif //REALTIMEGARBAGECOLLECTOR_GCINDEX_H

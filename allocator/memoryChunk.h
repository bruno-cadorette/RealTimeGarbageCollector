//
// Created by bruno on 30/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_PAGE_H
#define REALTIMEGARBAGECOLLECTOR_PAGE_H

#include <string>
#include <iostream>
#include <bitset>
#include <array>

//Inspired by Boehm's collector
class memoryChunk {
    //Must find a way to get this information without constant
    static const int PAGE_SIZE = 4096;

    static const int ELEM_SIZE = 2;
    static const int NUMBER_OF_ELEM = 16336/(1 + 4 * ELEM_SIZE);
    struct memoryChunkHeader{
        u_int16_t size;
        memoryChunk* next;
        std::bitset<NUMBER_OF_ELEM> markBits;
    } header;


    //Someone fix this, number of bit must be equal to the max number of elements
    static const std::size_t memorySize = PAGE_SIZE - sizeof(memoryChunkHeader);
    std::array<char, memorySize> memory;

    std::size_t getIndexFromPtr(std::size_t ptr) const{
        auto m = (std::size_t)memory.data();
        if (ptr > m && ptr  < m + sizeof(memory)){
            return ptr - m;
        }
        else{
            throw "trying to access an out of bound address";
        }
    }

public:
    memoryChunk(u_int16_t size) {
        header.size = size;
        header.next = nullptr;
    }

    memoryChunk(){
        static_assert(sizeof(memoryChunk) == PAGE_SIZE, "Bad size of memory chunk");
        //static_assert(sizeof(memory) / ELEM_SIZE == NUMBER_OF_ELEM, "Bad number of elem");
    }

    char* startOfMemory() noexcept {
        return memory.data();
    }
    std::size_t getMemorySize() const noexcept {
        return memorySize;
    }
};

#endif //REALTIMEGARBAGECOLLECTOR_PAGE_H

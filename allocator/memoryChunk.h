//
// Created by bruno on 30/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_PAGE_H
#define REALTIMEGARBAGECOLLECTOR_PAGE_H

#include <string>
#include <iostream>
#include <bitset>

//Inspired by Boehm's collector
class memoryChunk {
    //Must find a way to get this information without constant
    static const int PAGE_SIZE = 4096;
    static const int NUMBER_OF_ELEM = 3624;
    static const int ELEM_SIZE = 1;
    struct memoryChunkHeader{
        u_int16_t size;
        memoryChunk* next;
    } header;

    //Someone fix this, number of bit must be equal to the max number of elements
    std::bitset<NUMBER_OF_ELEM> markBits;
    char memory[PAGE_SIZE - sizeof(memoryChunkHeader) - sizeof(markBits)];

    std::size_t getIndexFromPtr(std::size_t ptr) const{
        auto m = (std::size_t)memory;
        if (ptr > m && ptr  < m + sizeof(memory)){
            return ptr - m;
        }
        else{
            throw "trying to access an out of bound address";
        }
    }

    bool isFull() const {
        return false;
    }

public:
    memoryChunk(u_int16_t size) {
        header.size = size;
        header.next = nullptr;
    }

    memoryChunk(){
        static_assert(sizeof(memoryChunk) == PAGE_SIZE, "Bad size of memory chunk");
        static_assert(sizeof(memory) / ELEM_SIZE == NUMBER_OF_ELEM, "Bad number of elem");
    }

    std::size_t getSize() const {
        return header.size;
    }
    //no need to specify the size because it always allocate the same size
    void* allocate(){
        if(isFull()){
            if(header.next == nullptr){
                header.next = new memoryChunk(header.size);
                return header.next->allocate();
            }
        }
    }
};

#endif //REALTIMEGARBAGECOLLECTOR_PAGE_H

//
// Created by bruno on 30/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_PAGE_H
#define REALTIMEGARBAGECOLLECTOR_PAGE_H

#include <string>
#include <iostream>
#include <assert.h>

//Inspired by Boehm collector
template <unsigned int CHUNK_SIZE, unsigned int OBJ_SIZE>
class memoryChunk {
    static_assert(CHUNK_SIZE%4096 == 0, "a memory chunk must be a multiple of 4096!");
    struct memoryChunkHeader{
        u_int16_t size;
        memoryChunk<CHUNK_SIZE, OBJ_SIZE>* next;
        bool mark[CHUNK_SIZE/OBJ_SIZE];//on peut faire 8 fois mieux
    } header;

    char memory[CHUNK_SIZE-sizeof(memoryChunkHeader)];

    std::size_t getIndexFromPtr(std::size_t ptr) const{
        auto m = (std::size_t)memory;
        if (ptr > m && ptr  < m + sizeof(memory)){
            return ptr - m;
        }
        else{
            throw "trying to mark a out of bound address";
        }

    }

public:
    memoryChunk() {
        header.size = 0;
        header.next = this;
        header.mark = false;
        static_assert(sizeof(*this) == CHUNK_SIZE, "The memory chunk is not of size N");
    }

    void mark(std::size_t ptr, bool markValue) {
        header.mark[getIndexFromPtr(ptr)] = markValue;

    }
    bool isMarked(std::size_t ptr) const {
        return header.mark[getIndexFromPtr(ptr)];
    }

    size_t mask() const noexcept{
        return (std::size_t)memory & ~(CHUNK_SIZE-1);
    }

};

#endif //REALTIMEGARBAGECOLLECTOR_PAGE_H

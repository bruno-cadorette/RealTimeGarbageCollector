//
// Created by bruno on 30/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_PAGE_H
#define REALTIMEGARBAGECOLLECTOR_PAGE_H

#include <string>
#include <iostream>
#include <assert.h>

//Inspired by Boehm's collector
class memoryChunk {
    static const int CHUNK_SIZE = 4096;
    struct memoryChunkHeader{
        u_int16_t size;
        memoryChunk* next;
    } header;

    char memory[CHUNK_SIZE - sizeof(memoryChunkHeader)];

    std::size_t getIndexFromPtr(std::size_t ptr) const{
        auto m = (std::size_t)memory;
        if (ptr > m && ptr  < m + sizeof(memory)){
            return ptr - m;
        }
        else{
            throw "trying to access an out of bound address";
        }
    }

    u_int16_t sizeOfMarkBits() const {
        return (CHUNK_SIZE - sizeof(memoryChunkHeader)) / header.size / 8;
    }

    //start of memory = header + markbits (1 bit per obj)
    //
    const char* startOfMemory() const {
        return memory + sizeOfMarkBits();
    }

    bool isFull() const {
        return false;
    }

public:
    memoryChunk(u_int16_t size) {
        header.size = size;
        header.next = nullptr;
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

/*
    void mark(std::size_t ptr, bool markValue) {
        header.mark[getIndexFromPtr(ptr)] = markValue;

    }
    bool isMarked(std::size_t ptr) const {
        return header.mark[getIndexFromPtr(ptr)];
    }

    size_t mask() const noexcept{
        return (std::size_t)memory & ~(CHUNK_SIZE-1);
    }
*/
};

#endif //REALTIMEGARBAGECOLLECTOR_PAGE_H

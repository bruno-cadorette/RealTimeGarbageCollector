//
// Created by bruno on 30/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_PAGE_H
#define REALTIMEGARBAGECOLLECTOR_PAGE_H

#include <string>
#include <iostream>
#include <bitset>
#include <array>

const std::size_t PAGE_SIZE = 4096;
template <std::size_t ELEM_SIZE>
class memoryChunk {
    char m[PAGE_SIZE];

public:
    static constexpr std::size_t numberOfElems(){
        return PAGE_SIZE / ELEM_SIZE;
    }
    memoryChunk(){
        static_assert(ELEM_SIZE % 2 == 0 && PAGE_SIZE > ELEM_SIZE, "Bad N value");
    }
    constexpr char* get(std::size_t i){
        return m + i;
    }
    constexpr char* begin(){
        return m;
    }
    constexpr char* end(){
        return get(PAGE_SIZE - 1);
    }
};

struct memoryChunkHeader{
    virtual char* startOfMemory() = 0;
    std::size_t getMemorySize(){
        return PAGE_SIZE;
    }
    virtual bool isValid(std::size_t) = 0;
};

//Inspired by Boehm's collector
template <std::size_t ELEM_SIZE>
class memoryChunkHeaderImpl : public memoryChunkHeader {
    memoryChunk<ELEM_SIZE>* memory;
    std::bitset<memoryChunk<ELEM_SIZE>::numberOfElems()> markBits;
public:
    char* startOfMemory(){
        return memory->get(0);
    }
    bool isValid(std::size_t addr){
        return false; //TODO

    }
};

#endif //REALTIMEGARBAGECOLLECTOR_PAGE_H

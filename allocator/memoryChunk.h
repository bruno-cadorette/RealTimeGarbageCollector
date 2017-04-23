//
// Created by bruno on 30/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_PAGE_H
#define REALTIMEGARBAGECOLLECTOR_PAGE_H

#include <string>
#include <iostream>
#include <bitset>
#include <array>
#include "objectSize.h"

template <std::size_t ELEM_SIZE>
class memoryChunk {
    char m[objectSize::PAGE_SIZE];

public:
    memoryChunk(){
        static_assert(objectSize::isValidSize(ELEM_SIZE), "Bad ELEM_SIZE value");
    }
    constexpr char* get(std::size_t i){
        return m + i;
    }
    constexpr char* begin(){
        return m;
    }
    constexpr char* end(){
        return get(objectSize::PAGE_SIZE - 1);
    }
    bool ptrPointToValidObject(char* ptr){
        auto distance = std::distance(m, ptr);
        return distance >= 0 && distance < objectSize::PAGE_SIZE && distance % ELEM_SIZE == 0;
    }
};

struct memoryChunkHeader{
    virtual char* startOfMemory() = 0;
    std::size_t getMemorySize(){
        return objectSize::PAGE_SIZE;
    }
    virtual void mark(char* ptr) = 0;
    virtual std::size_t getPtrSize() = 0;
    virtual bool isValid(char*) = 0;
    virtual bool isMarked(char* ptr) = 0;
    virtual bool canBeDeleted() = 0;
    virtual void unMarks() = 0;
    virtual ~memoryChunkHeader(){};
};

//Inspired by Boehm's collector
template <std::size_t ELEM_SIZE>
class memoryChunkHeaderImpl : public memoryChunkHeader {

    memoryChunk<ELEM_SIZE>* memory;
    std::bitset<objectSize::numberOfElems(ELEM_SIZE)> markBits;
public:

    bool canBeDeleted(){
        return markBits.none();
    }

    void unMarks(){
        markBits.reset();
    }
    char* startOfMemory(){
        return memory->begin();
    }
    void mark(char* ptr){
        markBits.set(ptr - startOfMemory());
    }
    bool isMarked(char* ptr){
        return markBits[ptr - startOfMemory()];
    }
    bool isValid(char* ptr){
        return memory->ptrPointToValidObject(ptr);
    }
    std::size_t getPtrSize(){
        return ELEM_SIZE;
    }
    memoryChunkHeaderImpl() : memory{new memoryChunk<ELEM_SIZE>}{

    }
    ~memoryChunkHeaderImpl(){
        delete memory;
    }
};

#endif //REALTIMEGARBAGECOLLECTOR_PAGE_H

//
// Created by bruno on 22/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_ENCODING_H
#define REALTIMEGARBAGECOLLECTOR_ENCODING_H


#include <cstdint>
const static uint64_t ENCODING = 0xbeefbeef00000;
const static uint64_t MAX_HEAP_SIZE = 65536;
class encoding {

public:

    static uint64_t encode(uint64_t ptr);
    static uint64_t decode(uint64_t ptr);
    static bool isValidPtr(uint64_t ptr);
};


class encodedPtr{
    uint64_t ptr;
public:
    bool isValid();
    encodedPtr(){}
    encodedPtr(uint64_t ptr) : ptr{ptr + ENCODING}{}
    encodedPtr& operator= (const uint64_t& rhs){
        ptr = rhs + ENCODING;
        return *this;
    }
    uint64_t decode() const{
        return ptr - ENCODING;
    }
};

#endif //REALTIMEGARBAGECOLLECTOR_ENCODING_H

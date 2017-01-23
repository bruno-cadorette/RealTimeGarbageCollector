//
// Created by bruno on 22/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_ENCODING_H
#define REALTIMEGARBAGECOLLECTOR_ENCODING_H


#include <cstdint>

class encoding {
    const static uint64_t ENCODING = 0xbeefbeef00000;
public:
    const static uint64_t MAX_HEAP_SIZE = 65536;
    static uint64_t encode(uint64_t ptr);
    static uint64_t decode(uint64_t ptr);
    static bool isValidPtr(uint64_t ptr);
};


#endif //REALTIMEGARBAGECOLLECTOR_ENCODING_H

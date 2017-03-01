//
// Created by bruno on 27/01/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_ADDRESSRANGE_H
#define REALTIMEGARBAGECOLLECTOR_ADDRESSRANGE_H


#include <cstddef>
#include <limits>
#include <algorithm>

class addressRange {
    std::size_t minPtr = std::numeric_limits<std::size_t>::max();
    std::size_t maxPtr = std::numeric_limits<std::size_t>::min();

public:
    void* newPrt(void* ptr, std::size_t size);
    bool inRange(std::size_t ptr);
};


#endif //REALTIMEGARBAGECOLLECTOR_ADDRESSRANGE_H

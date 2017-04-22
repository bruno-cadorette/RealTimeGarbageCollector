//
// Created by bruno on 27/01/17.
//

#include "addressRange.h"

bool addressRange::inRange(std::size_t ptr) {
    return minPtr <= ptr && ptr < maxPtr;
}

void* addressRange::newPrt(void* ptr, std::size_t size) {
    auto address = reinterpret_cast<std::size_t>(ptr);
    minPtr = std::min(minPtr, address);
    maxPtr = std::max(maxPtr, address + size);
    return ptr;
}

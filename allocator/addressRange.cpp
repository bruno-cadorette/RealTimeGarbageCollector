//
// Created by bruno on 27/01/17.
//

#include "addressRange.h"

bool addressRange::inRange(std::size_t ptr) {
    return minPtr <= ptr && ptr <= maxPtr;
}

void addressRange::newPrt(std::size_t ptr) {
    minPtr = std::min(minPtr, ptr);
    maxPtr = std::max(maxPtr, ptr);
}

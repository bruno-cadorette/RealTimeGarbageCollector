//
// Created by bruno on 22/01/17.
//

#include "encoding.h"

uint64_t encoding::encode(uint64_t ptr) {
    return ptr + ENCODING;
}

uint64_t encoding::decode(uint64_t ptr) {
    return ptr - ENCODING;
}

bool encoding::isValidPtr(uint64_t ptr) {
    return ptr > ENCODING && ptr - ENCODING < MAX_HEAP_SIZE ;
}

bool encodedPtr::isValid() {
    return ptr > ENCODING && ptr - ENCODING < MAX_HEAP_SIZE;
}
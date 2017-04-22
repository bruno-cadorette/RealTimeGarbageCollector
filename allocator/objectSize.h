//
// Created by bruno on 02/03/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_OBJECTSIZE_H
#define REALTIMEGARBAGECOLLECTOR_OBJECTSIZE_H


#include <cstddef>
#include "../dependentArray.h"

struct objectSize {
public:
    static const std::size_t PAGE_SIZE = 4096;
    static constexpr bool isValidSize(std::size_t size){
        return size <= PAGE_SIZE / 2 && size % 4 == 0;
    }
    static constexpr std::size_t numberOfElems(std::size_t size){
        return PAGE_SIZE / size;
    }

};


#endif //REALTIMEGARBAGECOLLECTOR_OBJECTSIZE_H

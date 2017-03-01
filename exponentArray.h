//
// Created by bruno on 27/02/17.
//

#ifndef REALTIMEGARBAGECOLLECTOR_EXPONENTARRAY_H
#define REALTIMEGARBAGECOLLECTOR_EXPONENTARRAY_H

#include <array>
#include <utility>

template <template<unsigned int> class Sub, class Parent, std::size_t N>
class exponentArray{
    template<std::size_t... index>
    constexpr std::array<Parent*, sizeof...(index)> generate_helper(std::index_sequence<index...>) {
        return {{static_cast<Parent*>(new Sub<1 << index>)... }};
    }
    template<typename Indices = std::make_index_sequence<N>>
    constexpr std::array<Parent*, N> generate_Xs() {
        return generate_helper(Indices());
    }

    std::array<Parent*, N> array = generate_Xs();

    constexpr std::array<Parent*, N> data(){
        return array;
    };

};


#endif //REALTIMEGARBAGECOLLECTOR_EXPONENTARRAY_H

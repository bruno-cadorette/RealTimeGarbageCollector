#include <iostream>
#include <new>
#include <memory>
#include <algorithm>
#include "allocator/freeList.h"

using namespace std;

struct Y {
    virtual void show() const = 0;
    virtual ~Y() = default;
};

template <std::size_t N>
struct X : public Y {
    void show() const override {
        std::cout << N << std::endl;
    }
};

template<std::size_t... index>
std::array<Y*, sizeof...(index)> generate_helper(std::index_sequence<index...>) {
    return {{ new X<1 << index>... }};
}
template<int N, typename Indices = std::make_index_sequence<N>>
std::array<Y*, N> generate_Xs() {
    return generate_helper(Indices());
}


int main() {
    auto a = generate_Xs<sizeof(X<6>)>();

    std::for_each(std::begin(a), std::end(a), [](const auto &y) { y->show(); });

    std::for_each(std::begin(a), std::end(a), [](auto &y) { delete y; });
}



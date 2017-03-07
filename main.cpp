#include <iostream>
#include <new>
#include <memory>
#include <algorithm>
#include "allocator/freeList.h"

using namespace std;
struct Foo{virtual std::size_t getN() = 0;};
template<std::size_t N>
struct FooN : public Foo{std::size_t getN(){return N;}};
struct indexerId {
    static constexpr std::size_t mapIndex(std::size_t x){
        return x * 2;
    }
};
void dependentArrayTest(){
    const std::size_t N = 100;
    dependentArray<FooN, Foo, N,indexerId> fooArr;
    assert(fooArr.size() == N);
    vector<std::size_t> a, b;
    for (std::size_t i = 0; i < N; ++i) {
        a.push_back(i*2);
    }
    auto arr = fooArr.getArray();
    std::transform(arr.begin(), arr.end(), b.begin(), [](Foo* f){
        return f->getN();
    });
    assert(std::equal(a.begin(), a.end(), b.begin()));
}



int main() {
    dependentArrayTest();
}



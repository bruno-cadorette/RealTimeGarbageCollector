#include "tests.h"
#include "Gc.h"

#include <cstddef>
#include <random>
#include <sstream>
#include <vector>
#include <string>
#include "sha256.h"

using namespace std;


struct A {
    std::size_t x;
    A(std::size_t x) : x{x} {}
};

struct B {
    Gc<A> a;
    B(std::size_t x) : a{MakeGc<A>(x)} {}
};

struct C {
    Gc<B> b;
    char a[1024];
    C(std::size_t x) : b{MakeGc<B>(x)} {}
};

void test_small_allocations(std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        auto b = MakeRootGc<B>(i);
        for (std::size_t j = 0; j < 50; ++j) {
            b->a = MakeGc<A>(j);
        }
    }
}

void test_big_allocations(std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        auto c = MakeRootGc<C>(i);
        for (std::size_t j = 0; j < size; ++j) {
            c->b = MakeGc<B>(j);
        }
    }
}

// CSCoins miner (CSGames competition)
bool try_nonce(int64_t nonce);

void test_realistic(std::size_t) {
    // cscoins list sort miner
    for (int64_t nonce = 0; !try_nonce(nonce); ++nonce) {}
}

string sha256(const string& src) {
    SHA256 h;
    return h(src);
}

template <typename T>
T swap_endian(T u) {
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");

    union {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++)
        dest.u8[k] = source.u8[sizeof(T) - k - 1];

    return dest.u;
}

int64_t seed_from_hash(const string& hash) {
    string prefix(begin(hash), next(begin(hash), 16)); // 8 bytes
    stringstream converter(prefix);
    int64_t seed;
    converter >> std::hex >> seed;
    return swap_endian(seed);
}

void feed_prng(mt19937_64& mt, const char* previous, int64_t nonce) {
    stringstream ss;
    ss << previous << nonce;
    string hash = sha256(ss.str());
    auto seed = seed_from_hash(hash);
    mt.seed(seed);
}

bool try_nonce(int64_t nonce) {
    const string target = "fff";
    const char* previous_hash = "0000000000000000000000000000000000000000000000000000000000000000";
    const int nb_elements = 10;
    stringstream ss;
    mt19937_64 mt;
    vector<RootGc<int64_t>> values;
    feed_prng(mt, previous_hash, nonce);
    values.resize(nb_elements);
    for (int i = 0; i < nb_elements; ++i) values[i] = MakeRootGc<int64_t>(mt());

    sort(begin(values), end(values), [](auto a, auto b) { return *a < *b; });

    ss.str(string());
    for (auto& v : values) { ss << *v; }
    auto hash = sha256(ss.str());
    return hash.compare(0, target.size(), target) == 0;
}

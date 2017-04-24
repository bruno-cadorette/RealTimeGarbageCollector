#ifndef REALTIMEGARBAGECOLLECTOR_TESTS_H
#define REALTIMEGARBAGECOLLECTOR_TESTS_H

#include <cstddef>

void test_small_allocations(std::size_t size);
void test_big_allocations(std::size_t size);
void test_realistic(std::size_t size);

#endif

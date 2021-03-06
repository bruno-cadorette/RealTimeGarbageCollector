#ifndef REALTIMEGARBAGECOLLECTOR_GCSTATS_H
#define REALTIMEGARBAGECOLLECTOR_GCSTATS_H

#include <chrono>
#include <vector>
#include <algorithm>
#include <cassert>

using StatsClock = std::chrono::high_resolution_clock;
using CollectDuration = StatsClock::duration;

class garbageCollector;

template <class T>
class Metric {
public:
    void update(const T& stat) {
        max_ = std::max(stat, max_);
        min_ = std::min(stat, min_);
        total_ += stat;
        ++count_;
    }

    const T& min() const { return min_; }
    const T& max() const { return max_; }
    T avg() const {
        assert(count());
        return total() / count();
    }
    std::size_t count() const { return count_; }
    T total() const { return total_; }

private:
    T min_;
    T max_;
    T total_;
    std::size_t count_;
};

class GcStats {
public:
    GcStats();
    void recordCollectDuration(CollectDuration duration) noexcept;
    void recordMemoryUsage(float usage) noexcept;

    Metric<CollectDuration> getTimeStats() const { return time; }
    Metric<float> getMemoryStats() const { return memory; }
    std::size_t getBustedTimeConstraints() const { return bustedTimeConstraints; }

private:
    Metric<CollectDuration> time;
    Metric<float> memory;
    std::size_t bustedTimeConstraints;
};


class GcCollectMonitor {
public:
    GcCollectMonitor(const garbageCollector& gc, GcStats& stats);
    ~GcCollectMonitor();

private:
    void recordMemory();
    StatsClock::time_point start;
    GcStats& stats;
    const garbageCollector& gc;
};

#endif

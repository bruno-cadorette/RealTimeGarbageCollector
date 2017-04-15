#ifndef REALTIMEGARBAGECOLLECTOR_GCSTATS_H
#define REALTIMEGARBAGECOLLECTOR_GCSTATS_H

#include <chrono>
#include <vector>
#include <algorithm>

using StatsClock = std::chrono::high_resolution_clock;
using CollectDuration = StatsClock::duration;

class garbageCollector;

// Individual statistic
template <class T>
class GcStat {
public:
    void update(const T& stat) {
        max_ = std::max(stat, max_);
        min_ = std::min(stat, min_);
        total += stat;
        ++count_;
    }

    const T& min() const { return min_; }
    const T& max() const { return max_; }
    T avg() const { return total / count(); }
    std::size_t count() const { return count_; }

private:
    T min_;
    T max_;
    T total;
    std::size_t count_;
};

class GcStats {
public:
    GcStats();
    void recordCollectDuration(CollectDuration duration) noexcept;
    void recordMemoryUsage(float usage) noexcept;

    GcStat<CollectDuration> getTimeStats() const { return time; }
    GcStat<float> getMemoryStats() const { return memory; }
    std::size_t getBustedTimeConstraints() const { return bustedTimeConstraints; }

private:
    GcStat<CollectDuration> time;
    GcStat<float> memory;
    std::size_t bustedTimeConstraints;
};


class GcCollectMonitor {
public:
    GcCollectMonitor(const garbageCollector& gc, GcStats& stats);
    ~GcCollectMonitor();

private:
    StatsClock::time_point start;
    GcStats& stats;
    const garbageCollector& gc;
};

#endif

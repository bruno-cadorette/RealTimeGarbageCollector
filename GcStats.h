#ifndef REALTIMEGARBAGECOLLECTOR_GCSTATS_H
#define REALTIMEGARBAGECOLLECTOR_GCSTATS_H

#include <chrono>
#include <vector>

using StatsClock = std::chrono::high_resolution_clock;
using CollectDuration = StatsClock::duration;

class GcStats {
public:
    GcStats();
    void recordCollectDuration(CollectDuration duration) noexcept;

    std::size_t getCollectionsCount() const { return collectionsCount; }
    CollectDuration getAverageDuration() const { return timeSpent / collectionsCount; }
    CollectDuration getMaxDuration() const { return maxTime; }
    std::size_t getBustedTimeConstraints() const { return bustedTimeConstraints; }

private:
    CollectDuration timeSpent;
    CollectDuration maxTime;
    std::size_t collectionsCount;
    std::size_t bustedTimeConstraints;
};


class GcCollectMonitor {
public:
    GcCollectMonitor(GcStats& stats);
    ~GcCollectMonitor();

private:
    StatsClock::time_point start;
    GcStats& stats;
};

#endif

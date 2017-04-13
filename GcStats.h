#ifndef REALTIMEGARBAGECOLLECTOR_GCSTATS_H
#define REALTIMEGARBAGECOLLECTOR_GCSTATS_H

#include <chrono>
#include <vector>

using StatsClock = std::chrono::high_resolution_clock;
using CollectDuration = StatsClock::duration;

class GcStats {
public:
    using CollectDurations = std::vector<CollectDuration>;

    GcStats();
    void recordCollectDuration(CollectDuration duration) noexcept;

    const CollectDurations& getCollectDurations() const { return collections; }

private:
    CollectDurations collections;
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

#include "GcStats.h"

const auto DURATION_CONSTRAINT = std::chrono::milliseconds(5);

GcStats::GcStats()
    : timeSpent{}, maxTime{}, collectionsCount{}, bustedTimeConstraints{} {
}

void GcStats::recordCollectDuration(CollectDuration duration) noexcept {
    timeSpent += duration;
    ++collectionsCount;

    if (duration > maxTime) {
        maxTime = duration;
    }

    if (duration > DURATION_CONSTRAINT) {
        ++bustedTimeConstraints;
    }
}

GcCollectMonitor::GcCollectMonitor(GcStats& stats)
    : start{StatsClock::now()}, stats{stats} {
}

GcCollectMonitor::~GcCollectMonitor() {
    const auto end = StatsClock::now();
    stats.recordCollectDuration(end - start);
}

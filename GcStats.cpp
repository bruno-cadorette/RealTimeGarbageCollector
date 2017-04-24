#include "garbageCollector.h"

#include "GcStats.h"

const auto DURATION_CONSTRAINT = std::chrono::milliseconds(1);

GcStats::GcStats()
    : time{}, bustedTimeConstraints{} {
}

void GcStats::recordCollectDuration(CollectDuration duration) noexcept {
    time.update(duration);

    if (duration > DURATION_CONSTRAINT) {
        ++bustedTimeConstraints;
    }
}

GcCollectMonitor::GcCollectMonitor(const garbageCollector& gc, GcStats& stats)
    : start{StatsClock::now()}, stats{stats}, gc{gc} {
}

GcCollectMonitor::~GcCollectMonitor() {
    const auto end = StatsClock::now();
    stats.recordCollectDuration(end - start);
}

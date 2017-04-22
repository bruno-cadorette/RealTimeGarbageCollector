#include "garbageCollector.h"

#include "GcStats.h"

const auto DURATION_CONSTRAINT = std::chrono::milliseconds(5);

GcStats::GcStats()
    : time{}, memory{}, bustedTimeConstraints{} {
}

void GcStats::recordCollectDuration(CollectDuration duration) noexcept {
    time.update(duration);

    if (duration > DURATION_CONSTRAINT) {
        ++bustedTimeConstraints;
    }
}

void GcStats::recordMemoryUsage(float usage) noexcept {
    memory.update(usage);
}

GcCollectMonitor::GcCollectMonitor(const garbageCollector& gc, GcStats& stats)
    : start{StatsClock::now()}, stats{stats}, gc{gc} {
}

GcCollectMonitor::~GcCollectMonitor() {
    const auto end = StatsClock::now();
    stats.recordCollectDuration(end - start);

    const auto memory = gc.getManagedMemorySize();
    if (memory > 0) { // only interested when our GC is managing memory
        const auto usage = static_cast<float>(gc.getMemoryOverhead()) / memory;
        stats.recordMemoryUsage(usage);
    }
}

#include "GcStats.h"

#include <iostream>

// We can't allow stats collection to throw (called in dtors) or be
// undeterministic, so we collect a max amount of stats.
const int MAX_COLLECT_STATS = 5000;

GcStats::GcStats() {
    collections.reserve(MAX_COLLECT_STATS);
}

void GcStats::recordCollectDuration(CollectDuration duration) noexcept {
    if (collections.size() < MAX_COLLECT_STATS) {
        collections.push_back(duration);
    } else {
        // We're collecting too many times for our bookkeeping capabilities,
        // report as a warning and ignore the saved timing (not dramatic)
        std::cerr << "WARNING: Tried to save a collection time, but history is full."
                  << std::endl;
    }
}

GcCollectMonitor::GcCollectMonitor(GcStats& stats)
    : start{StatsClock::now()}, stats{stats} {
}

GcCollectMonitor::~GcCollectMonitor() {
    const auto end = StatsClock::now();
    stats.recordCollectDuration(end - start);
}

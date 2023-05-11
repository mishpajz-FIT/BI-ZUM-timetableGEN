#include <utility>

#include "importers.h"

ImporterException::ImporterException(std::string message) : msg(std::move(message)) { }

void Importer::calculateCollisions(Semester & semester) {

    using IntervalEntry = std::pair<TimeInterval, std::shared_ptr<Entry>>;
    std::vector<IntervalEntry> intervals;

    for (auto & coursePtr : semester.coursesPtrs) {
        for (auto & schedulePtr : coursePtr->schedulesPtrs) {
            for (auto & entryPtr : schedulePtr.second->entriesPtrs) {
                for (auto & interval : entryPtr->timeslots) {
                    intervals.push_back(std::make_pair(interval, entryPtr));
                }
            }
        }
    }

    std::sort(intervals.begin(), intervals.end(), [ ] (const IntervalEntry & lhs, const IntervalEntry & rhs) -> bool {
        return lhs.first < rhs.first;
        });

    for (auto it = intervals.begin(); it != intervals.end(); it++) {
        auto interIt = it + 1;
        while (interIt != intervals.end()
            && ((interIt->first.startTime < it->first.endTime) && (it->first.day == interIt->first.day))) {

            if (it->first.parity == interIt->first.parity
                || (it->first.parity == TimeInterval::Parity::Both || interIt->first.parity == TimeInterval::Parity::Both)) {

                it->second->collisions.insert(interIt->second);
                interIt->second->collisions.insert(it->second);
            }

            interIt++;
        }

    }
}

Semester Importer::import() {
    Semester result = load();
    calculateCollisions(result);
    return result;
}

const char * ImporterException::what() const noexcept {
    return msg.c_str();
}
#include "Custom/StdoutOutputter.h"

StdoutOutputter::StdoutOutputter() {

    namesOfDays[0] = "monday";
    namesOfDays[1] = "tuesday";
    namesOfDays[2] = "wednesday";
    namesOfDays[3] = "thursday";
    namesOfDays[4] = "friday";
    namesOfDays[5] = "saturday";
    namesOfDays[6] = "sunday";

    parityName = "parity";
    parityOdd = "odd";
    parityEven = "even";
}

#define DAY_SEPARATOR '*'
#define ENTRY_SEPARATOR '_'
#define SEPARATOR_LENGTH 55

void StdoutOutputter::output(const std::vector<EvolutionResult> & result) {

    // Get all intervals and sort them by start time
    using IntervalEntry = std::pair<TimeInterval, EvolutionResult>;
    std::vector<IntervalEntry> intervals;

    for (auto & entry : result) {
        for (auto & interval : entry.second->timeslots) {
            intervals.push_back(std::make_pair(interval, entry));
        }
    }


    std::sort(intervals.begin(), intervals.end(), [ ] (const IntervalEntry & lhs, const IntervalEntry & rhs) -> bool {
        return lhs.first < rhs.first;
        });

    size_t lastDay = -1;
    auto prevIt = intervals.end();
    for (auto it = intervals.begin(); it != intervals.end(); it++) {
        TimeInterval interval = it->first;
        EntryAddress address = it->second.first;
        std::shared_ptr<Entry> entry = it->second.second;

        size_t day = static_cast<size_t>(interval.day);

        if (lastDay != day) {
            lastDay = day;
            std::cout << "\n";
            std::cout << std::setw(SEPARATOR_LENGTH) << std::setfill(DAY_SEPARATOR);
            std::cout << "\n";
            std::cout << namesOfDays[day] << "\n";
            std::cout << std::setw(SEPARATOR_LENGTH) << std::setfill(DAY_SEPARATOR);
            std::cout << "\n";
        } else if (prevIt != intervals.end()) {
            size_t hourDifference = (prevIt->first.startTime.valueInMinutes() - it->first.endTime.valueInMinutes()) / 60;
            for (size_t i = 0; i < hourDifference; i++) {
                std::cout << "\n";
            }
        }

        std::cout << std::setw(SEPARATOR_LENGTH) << std::setfill(ENTRY_SEPARATOR);
        std::cout << "\n";
        std::cout << address.first << "\n";
        std::cout << address.second << "\n";
        std::cout << "\n";
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(interval.startTime.hour) << ":";
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(interval.startTime.minute) << " - ";
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(interval.endTime.hour) << ":";
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(interval.endTime.minute) << "\n";
        if (interval.parity == TimeInterval::Parity::Odd) {
            std::cout << parityName << ": " << parityOdd;
        } else if (interval.parity == TimeInterval::Parity::Even) {
            std::cout << parityName << ": " << parityEven;
        }
        std::cout << "\n";
        std::cout << entry->id << "\n";
        std::cout << entry->additionalInformation << "\n";
        std::cout << std::setw(SEPARATOR_LENGTH) << std::setfill(ENTRY_SEPARATOR);
        std::cout << "\n" << std::endl;
    }

}


CS_StdoutOutputter::CS_StdoutOutputter(): StdoutOutputter() {
    namesOfDays[0] = "pondělí";
    namesOfDays[1] = "útery";
    namesOfDays[2] = "středa";
    namesOfDays[3] = "čtvrtek";
    namesOfDays[4] = "pátek";
    namesOfDays[5] = "sobota";
    namesOfDays[6] = "neděle";

    parityName = "parita";
    parityOdd = "sudý";
    parityEven = "lichý";
}
#include "Custom/StdoutOutputter.h"

StdoutOutputter::StdoutOutputter() {

    // Day names mapping
    namesOfDays[0] = "monday";
    namesOfDays[1] = "tuesday";
    namesOfDays[2] = "wednesday";
    namesOfDays[3] = "thursday";
    namesOfDays[4] = "friday";
    namesOfDays[5] = "saturday";
    namesOfDays[6] = "sunday";

    // Parity mapping
    parityName = "parity";
    parityOdd = "odd";
    parityEven = "even";
}

// Visual separator for days
#define DAY_SEPARATOR '*'

// Visual separator for entries
#define ENTRY_SEPARATOR '_'

// Length of visual separator
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


    // Print all intervals by day
    size_t lastDay = -1;
    auto prevIt = intervals.end();
    for (auto it = intervals.begin(); it != intervals.end(); it++) {
        TimeInterval interval = it->first;
        EntryAddress address = it->second.first;
        std::shared_ptr<Entry> entry = it->second.second;

        size_t day = static_cast<size_t>(interval.day);

        if (lastDay != day) { // Print day header (if previous entry in another day)
            lastDay = day;
            std::cout << "\n";
            std::cout << std::setw(SEPARATOR_LENGTH) << std::setfill(DAY_SEPARATOR);
            std::cout << "\n";
            std::cout << namesOfDays[day] << "\n";
            std::cout << std::setw(SEPARATOR_LENGTH) << std::setfill(DAY_SEPARATOR);
            std::cout << "\n";
        } else if (prevIt != intervals.end()) { // Print spacing based on time difference between entries (in same day)
            size_t hourDifference = 0;
            if (prevIt->first.endTime.valueInMinutes() < it->first.startTime.valueInMinutes()) {
                hourDifference = (it->first.startTime.valueInMinutes() - prevIt->first.endTime.valueInMinutes()) / 60;
            }
            for (size_t i = 0; i <= hourDifference; i++) {
                std::cout << "\n";
            }
        }

        // Print Entry

        std::cout << std::setw(SEPARATOR_LENGTH) << std::setfill(ENTRY_SEPARATOR);
        std::cout << "\n";
        // Print Course and Schedule
        std::cout << address.first << "\n";
        std::cout << address.second << "\n";
        std::cout << "\n";
        //Print time interval informations
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(interval.startTime.hour) << ":";
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(interval.startTime.minute) << " - ";
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(interval.endTime.hour) << ":";
        std::cout << std::setw(2) << std::setfill('0') << static_cast<int>(interval.endTime.minute) << "\n";
        if (interval.parity == TimeInterval::Parity::Odd) { // Print parity information
            std::cout << parityName << ": " << parityOdd << "\n";
        } else if (interval.parity == TimeInterval::Parity::Even) {
            std::cout << parityName << ": " << parityEven << "\n";
        }
        std::cout << "\n";
        // Print additional informations
        std::cout << entry->id << "\n";
        std::cout << entry->additionalInformation << "\n";
        std::cout << std::setw(SEPARATOR_LENGTH) << std::setfill(ENTRY_SEPARATOR);
        std::cout << "\n" << std::endl;


        prevIt = it; // Assign iterator
    }

}

CS_StdoutOutputter::CS_StdoutOutputter() : StdoutOutputter() {

    // Day names mapping
    namesOfDays[0] = "pondělí";
    namesOfDays[1] = "útery";
    namesOfDays[2] = "středa";
    namesOfDays[3] = "čtvrtek";
    namesOfDays[4] = "pátek";
    namesOfDays[5] = "sobota";
    namesOfDays[6] = "neděle";

    // Parity mapping
    parityName = "parita";
    parityOdd = "sudý";
    parityEven = "lichý";
}
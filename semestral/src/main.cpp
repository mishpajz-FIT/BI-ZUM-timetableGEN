#include "Custom/FITCTUImporter.h"
#include "Custom/StdinAdjuster.h"
#include "evolution.h"

#include <iostream>
#include <stdio.h>

int main() {

    CS_FITCTUImporter importer("examples/example1.txt");
    Semester semester = importer.import();

    StdinAdjuster adjuster;
    Priorities priorities = adjuster(semester);

    Evolution evolution(semester, priorities);
    std::vector<std::pair<EntryAddress, std::shared_ptr<Entry>>> result = evolution.evolve();

    for (size_t i = 0; i < result.size(); i++) {
        std::cout << result[i].first.first << std::endl;
        std::cout << "    " << result[i].first.second << std::endl;
        std::cout << "        " << result[i].second->id << std::endl;
        for (auto & timeslot : result[i].second->timeslots) {
            std::cout << "         - " << static_cast<size_t>(timeslot.day) << std::endl;
            std::cout << "         \\ " << static_cast<int>(timeslot.startTime.hour) << ":" << static_cast<int>(timeslot.startTime.minute) << "-" << static_cast<int>(timeslot.endTime.hour) << ":" << static_cast<int>(timeslot.endTime.minute) << std::endl;
        }
    }

    return 0;
}
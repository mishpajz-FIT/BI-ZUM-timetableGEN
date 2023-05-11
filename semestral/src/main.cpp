#include "Custom/FITCTUImporter.h"
#include "Custom/StdinAdjuster.h"

#include <iostream>
#include <stdio.h>

int main() {

    CS_FITCTUImporter importer("examples/example1.txt");

    Semester result = importer.import();
    StdinAdjuster adjuster;
    Priorities priorities = adjuster(result);

    for (auto & course : result.coursesPtrs) {
        std::cout << course->name << std::endl;
        for (auto & schedule : course->schedulesPtrs) {
            std::cout << " " << schedule.first << std::endl;
            for (auto & entry : schedule.second->entriesPtrs) {
                std::cout << "   " << entry->id << std::endl;
            }
        }
    }

    return 0;
}
#include "Custom/FITCTUImporter.h"

#include <iostream>

int main() {

    CS_FITCTUImporter importer("examples/example1.txt");

    std::vector<Course> result = importer.import();

    for (auto & course : result) {
        std::cout << course.name << std::endl;
        for (auto & schedule : course.schedules) {
            std::cout << " " << schedule.first << std::endl;
            for (auto & entry : schedule.second.entries) {
                std::cout << "   " << entry.id << std::endl;
            }
        }
    }

    return 0;
}
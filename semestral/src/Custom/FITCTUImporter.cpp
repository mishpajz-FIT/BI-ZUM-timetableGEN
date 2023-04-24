#include "FITCTUImporter.h"

FITCTUImporter::FITCTUImporter(const std::string & filename): Importer(), file(filename) {

    if (!file.is_open() || !file.good()) {
        throw ImporterException("File couldn't be opened.");
    }
}

std::vector<Course> FITCTUImporter::import() {

    FITCTUImporter::ReadingStates state = ReadingStates::Course;

    Course currentCourse;
    Entry currentEntry;

    std::string line;
    while(std::getline(file, line)) {

        trim(line);

        if (line.empty()) {
            continue;
        }

        switch(state) {
            case ReadingStates::Course:
                currentCourse.name = line;
                state = ReadingStates::Entry;
                break;
            case ReadingStates::Entry:
                currentEntry.id = std::stoi(line);
                state = ReadingStates::Capacity;
                break;
            case ReadingStates::Capacity:
                std::regex capacityRegex("[0-9]+/[0-9]+");
                if (!std::regex_search(line, capacityRegex)) {
                    throw ImporterException("Missing capacity.");
                }
                break;
            case ReadingStates::TimeInterval:
                break;
            case ReadingStates::Parity:
                break;
            case ReadingStates::Lecturer:
                break;
            case ReadingStates::Location:
                break;
        }



    }


}
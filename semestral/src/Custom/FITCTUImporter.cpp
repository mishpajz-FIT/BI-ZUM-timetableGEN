#include "FITCTUImporter.h"

FITCTUImporter::FITCTUImporter(const std::string & filename):
    Importer(),
    file(filename),
    dayMapping(),
    capacityRegex(),
    timeRegex() {

    if (!file.is_open() || !file.good()) {
        throw ImporterException("File couldn't be opened.");
    }

    dayMapping["po"] = TimeInterval::Day::Monday;
    dayMapping["út"] = TimeInterval::Day::Tuesday;
    dayMapping["st"] = TimeInterval::Day::Wednesday;
    dayMapping["čt"] = TimeInterval::Day::Thursday;
    dayMapping["pá"] = TimeInterval::Day::Friday;
    dayMapping["so"] = TimeInterval::Day::Saturday;
    dayMapping["ne"] = TimeInterval::Day::Sunday;

    capacityRegex = std::regex("[0-9]+/[0-9]+", std::regex::optimize);
    timeRegex = std::regex("(po|út|st|čt|pá) ([0-9][0-9]):([0-9][0-9]) - ([0-9][0-9]):([0-9][0-9])", std::regex::optimize);

    parityMapping["(týden: Sudý)"] = TimeInterval::Parity::Even;
    parityMapping["(týden: Lichý)"] = TimeInterval::Parity::Odd;
}

std::vector<Course> FITCTUImporter::import() {

    std::vector<Course> result;

    FITCTUImporter::ReadingStates state = ReadingStates::Course;

    Course currentCourse;
    bool addedCourseToResult = false;
    std::string currentScheduleType;
    Entry currentEntry;

    std::string line;
    size_t lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
        trim(line);

        if (line.empty()) {

            if (state == ReadingStates::Id) {
                result.push_back(currentCourse);
                addedCourseToResult = true;

                currentCourse = Course();

                state = ReadingStates::Course;
            }

            continue;
        }

        switch (state) {
            case ReadingStates::Course: {
                    addedCourseToResult = false;
                    currentCourse.name = line;
                    state = ReadingStates::Id;
                    break;
                }
            case ReadingStates::Id: {
                    try {
                        currentEntry.id = std::stoul(line);
                    }
                    catch (...) {
                        throw ImporterException("Wrong format of ID in file.");
                    }
                    state = ReadingStates::Type;
                    break;
                }
            case ReadingStates::Type: {
                    currentScheduleType = line;
                    state = ReadingStates::Capacity;
                    break;
                }
            case ReadingStates::Capacity: {
                    if (!std::regex_match(line, capacityRegex)) {
                        throw ImporterException("Wrong format of capacity in file.");
                    }
                    currentEntry.additionalInformation.append(line);
                    currentEntry.additionalInformation.push_back('\n');
                    state = ReadingStates::Time;
                    break;
                }
            case ReadingStates::Time: {
                    std::smatch match;
                    if (!std::regex_match(line, match, timeRegex) || (match.size() != 6 || !dayMapping.contains(match[1].str()))) {
                        throw ImporterException("Wrong format of time in file.");
                    }

                    TimeInterval::Day day = dayMapping[match[1].str()];

                    TimeInterval::TimeStamp start(std::stoul(match[2].str()), std::stoul(match[3].str()));
                    TimeInterval::TimeStamp end(std::stoul(match[4].str()), std::stoul(match[5].str()));

                    std::fstream::pos_type pos = file.tellg();
                    if (!std::getline(file, line)) {
                        throw ImporterException("File missing required lines.");
                    }
                    trim(line);

                    TimeInterval::Parity parity = TimeInterval::Parity::Both;
                    if (parityMapping.contains(line)) {
                        parity = parityMapping[line];

                        lineCount++;
                        pos = file.tellg();
                        if (!std::getline(file, line)) {
                            throw ImporterException("File missing required lines.");
                        }
                        trim(line);
                    }

                    currentEntry.timeslots.emplace_back(day, start, end, parity);

                    if (std::regex_match(line, match, timeRegex)) {
                        file.seekg(pos, std::ios_base::beg);
                        break;
                    }
                    lineCount++;
                    state = ReadingStates::Additional;
                }
            case ReadingStates::Additional: {
                    currentEntry.additionalInformation.append(line);
                    currentEntry.additionalInformation.push_back('\n');

                    std::fstream::pos_type pos = file.tellg();
                    if (!std::getline(file, line)) {
                        state = ReadingStates::Id;
                        currentCourse.schedules[currentScheduleType].entries.push_back(currentEntry);
                        break;
                    }
                    trim(line);
                    file.seekg(pos, std::ios_base::beg);

                    if (line.empty() || is_number(line)) {
                        currentCourse.schedules[currentScheduleType].entries.push_back(currentEntry);

                        currentScheduleType.clear();
                        currentEntry = Entry();
                        state = ReadingStates::Id;
                    }
                }
        }
    }

    if (state != ReadingStates::Id && state != ReadingStates::Course) {

        throw ImporterException("File missing required lines.");
    }

    if (!addedCourseToResult) {
        result.push_back(currentCourse);
    }

    return result;
}
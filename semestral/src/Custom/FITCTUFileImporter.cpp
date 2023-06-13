#include "FITCTUFileImporter.h"

FITCTUFileImporter::FITCTUFileImporter(const std::string & filename) :
    Importer(),
    file(filename, std::ios::in),
    dayMapping(),
    capacityRegex(),
    timeRegex() {

    if (!file.is_open() || !file.good()) {
        throw ImporterException("File couldn't be opened.");
    }
}

Semester FITCTUFileImporter::load() {

    Semester result;

    // State machine state
    FITCTUFileImporter::ReadingStates state = ReadingStates::Course;

    // Allocate initial structures
    std::shared_ptr<Course> currentCourse(new Course());
    std::shared_ptr<Schedule> currentSchedule(new Schedule(currentCourse));
    std::shared_ptr<Entry> currentEntry(new Entry(0, currentSchedule));
    bool addedCourseToResult = false;

    std::string line;
    size_t lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
        trim(line);

        if (line.empty()) { // If empty line skip or allocate new structures based on state

            if (state == ReadingStates::Id) { // If reading ID and encontering 
            // empty line, change reading state to course
            // Threfore before file should contain empty lines before each new course
                result.coursesPtrs.push_back(currentCourse);
                currentCourse = std::make_shared<Course>();
                currentSchedule = std::make_shared<Schedule>(currentCourse);
                currentEntry = std::make_shared<Entry>(0, currentSchedule);
                addedCourseToResult = true;

                state = ReadingStates::Course;
            }

            continue;
        }

        // State machine actions
        switch (state) {
            case ReadingStates::Course: { // Parse course name and set it to course structure
                    addedCourseToResult = false;
                    currentCourse->name = line;
                    state = ReadingStates::Id;
                    break;
                }
            case ReadingStates::Id: { // Parse entry ID and set it to entry structure
                    try {
                        currentEntry->id = std::stoul(line);
                    }
                    catch (...) {
                        std::string exceptionMessage("Wrong format of ID in file: line ");
                        throw ImporterException((exceptionMessage + std::to_string(lineCount)).c_str());
                    }
                    state = ReadingStates::Schedule;
                    break;
                }
            case ReadingStates::Schedule: { // Read schedule the entry belongs to

                    // If course doesnt contain this structure, allocate it
                    if (!currentCourse->schedulesPtrs.contains(line)) {
                        currentCourse->schedulesPtrs[line] = std::make_shared<Schedule>(currentCourse, line);
                    }
                    // Set current schedule to schedule with this name
                    currentSchedule = currentCourse->schedulesPtrs[line];

                    state = ReadingStates::Capacity;
                    break;
                }
            case ReadingStates::Capacity: { // Parse capacity
                    if (!std::regex_match(line, capacityRegex)) {
                        std::string exceptionMessage("Wrong format of capacity in file: line ");
                        throw ImporterException((exceptionMessage + std::to_string(lineCount)).c_str());
                    }
                    // Add it to additional information
                    currentEntry->additionalInformation.append(line);
                    currentEntry->additionalInformation.push_back('\n');
                    state = ReadingStates::Time;
                    break;
                }
            case ReadingStates::Time: { // Parse entry's time entries
                    std::smatch match;
                    if (!std::regex_match(line, match, timeRegex) || (match.size() != 6 || !dayMapping.contains(match[1].str()))) {
                        std::string exceptionMessage("Wrong format of time in file: line ");
                        throw ImporterException((exceptionMessage + std::to_string(lineCount)).c_str());
                    }

                    // Retrieve day, start and end time
                    TimeInterval::Day day = dayMapping[match[1].str()];
                    TimeInterval::TimeStamp start(std::stoul(match[2].str()), std::stoul(match[3].str()));
                    TimeInterval::TimeStamp end(std::stoul(match[4].str()), std::stoul(match[5].str()));

                    // Peek forward
                    std::fstream::pos_type pos = file.tellg();
                    if (!std::getline(file, line)) {
                        throw ImporterException("File missing required lines (file is too short).");
                    }
                    trim(line);

                    // Check if peeked line contains parity information
                    TimeInterval::Parity parity = TimeInterval::Parity::Both;
                    if (parityMapping.contains(line)) {
                        parity = parityMapping[line];

                        lineCount++;
                        pos = file.tellg(); // Peek additional line (consume already peaked line)
                        if (!std::getline(file, line)) {
                            throw ImporterException("File missing required lines (file is too short).");
                        }
                        trim(line);
                    }

                    // Store read timeslot
                    currentEntry->timeslots.emplace_back(day, start, end, parity);

                    // Check if peeked string contains another time entry, else continue reading additional information
                    if (std::regex_match(line, match, timeRegex)) {
                        file.seekg(pos, std::ios_base::beg); // Return peeked line back to stream
                        break; // Continue in next cycle with reading time again
                    }
                    lineCount++;
                    state = ReadingStates::Additional;
                }
            case ReadingStates::Additional: {
                    currentEntry->additionalInformation.append(line);
                    currentEntry->additionalInformation.push_back('\n');

                    // Peek next line
                    std::fstream::pos_type pos = file.tellg();
                    bool nextLine = false;
                    if (std::getline(file, line)) {
                        nextLine = true;
                        trim(line);
                        file.seekg(pos, std::ios_base::beg);
                    }

                    // If no next line, or next line is empty or next line is number,
                    // store the retrieved entry information
                    if (!nextLine || (line.empty() || is_number(line))) {
                        // Insert into correct schedule with correct index
                        currentEntry->indexInSchedule = currentSchedule->entriesPtrs.size();
                        currentEntry->schedule = currentSchedule;
                        currentSchedule->entriesPtrs.push_back(currentEntry);

                        // Allocate new, clean entry structure
                        currentEntry = std::make_shared<Entry>(0, currentSchedule);
                        state = ReadingStates::Id;
                    }
                }
        }
    }

    // Check if reading stopped in the middle of a entry
    if (state != ReadingStates::Id && state != ReadingStates::Course) {
        throw ImporterException("File missing required lines (file is too short).");
    }

    // Add last course to semester in case the file didn't end with an empty line
    // threfore it hasn't been added yet
    if (!addedCourseToResult) {
        result.coursesPtrs.push_back(currentCourse);
    }

    return result;
}

CS_FITCTUFileImporter::CS_FITCTUFileImporter(const std::string & filename) : FITCTUFileImporter(filename) {

    // Mapping for days
    dayMapping["po"] = TimeInterval::Day::Monday;
    dayMapping["út"] = TimeInterval::Day::Tuesday;
    dayMapping["st"] = TimeInterval::Day::Wednesday;
    dayMapping["čt"] = TimeInterval::Day::Thursday;
    dayMapping["pá"] = TimeInterval::Day::Friday;
    dayMapping["so"] = TimeInterval::Day::Saturday;
    dayMapping["ne"] = TimeInterval::Day::Sunday;

    // Regexes for capacity and time
    capacityRegex = std::regex("[0-9]+/[0-9]+", std::regex::optimize);
    timeRegex = std::regex("(po|út|st|čt|pá) ([0-9][0-9]):([0-9][0-9]) - ([0-9][0-9]):([0-9][0-9])", std::regex::optimize);

    // Mapping for parity
    parityMapping["(týden: Sudý)"] = TimeInterval::Parity::Even;
    parityMapping["(týden: Lichý)"] = TimeInterval::Parity::Odd;
}
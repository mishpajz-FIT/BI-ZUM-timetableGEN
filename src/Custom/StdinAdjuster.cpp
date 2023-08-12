#include "StdinAdjuster.h"

bool StdinAdjuster::checkInputForFail() {
    if (std::cin.fail()) {
        std::cin.clear(); // Clean stdin for next tries
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
        std::cout << "(!) Wrong input.\n";
        return true;
    }
    return false;
}

void StdinAdjuster::adjustBool(const std::string & infoText, bool & value) {
    bool active = true;
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    while (active) {
        // Print header
        std::cout << infoText << "\n\n";
        std::cout << "Current value: [" << std::boolalpha << value << "]\n";
        std::cout << "Enter new value (y/n) or 'q' to exit: \n";
        std::cout << std::string(STDIN_ADJUSTER_SEPARATOR_LENGTH, '_') << std::endl;

        // Retrieve input
        char choice;
        std::cin >> choice;

        // Validate input
        if (checkInputForFail()) {
            continue;
        }

        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
        switch (choice) {
            case 'y':
            case 'Y':
            case '1':
                value = true;
                continue;
            case 'n':
            case 'N':
            case '0':
                value = false;
                continue;
            case 'q':
            case 'Q':
                active = false;
                break;
            default:
                std::cout << "(!) Wrong input.\n";
                continue;
        }
    }
}

void StdinAdjuster::adjustHour(const std::string & infoText, uint8_t & value) {
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    while (true) {
        // Print header
        std::cout << infoText << "\n\n";
        std::cout << "Current value: [" << std::setw(2) << std::setfill('0') << static_cast<int>(value) << "]\n";
        std::cout << "Enter new value in 24h format (0 to disable) or 'q' to exit: \n";
        std::cout << std::string(STDIN_ADJUSTER_SEPARATOR_LENGTH, '_') << std::endl;

        // Retrieve input
        std::string newValueString;
        std::cin >> newValueString;

        // Validate input
        if (checkInputForFail()) {
            continue;
        }

        trim(newValueString);
        std::istringstream inputstringstream(newValueString);

        unsigned int newValue;
        char choice;

        if (inputstringstream >> newValue) { // Check for numerical value
            if (newValue >= 24) {
                newValue = 24;
            }

            value = static_cast<uint8_t>(newValue);
            std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
            continue;
        }

        inputstringstream.clear();
        if ((inputstringstream >> choice) && (choice == 'q' || choice == 'Q')) { // Check for quit signal
            return;
        }

        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    }
}

template <typename T>
void StdinAdjuster::adjustValue(const std::string & infoText, T & value) {
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    while (true) {
        // Print header
        std::cout << infoText << "\n\n";
        std::cout << "Current value: [" << value << "]\n";
        std::cout << "Enter new value or 'q' to exit: \n";
        std::cout << std::string(STDIN_ADJUSTER_SEPARATOR_LENGTH, '_') << std::endl;

        // Retrieve input
        std::string newValueString;
        std::cin >> newValueString;

        // Validate input
        if (checkInputForFail()) {
            continue;
        }

        trim(newValueString);
        std::istringstream inputstringstream(newValueString);

        T retrievedValue;
        char choice;

        if (inputstringstream >> retrievedValue) { // Check for required value
            value = retrievedValue;
            std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
            continue;
        }

        inputstringstream.clear();
        if ((inputstringstream >> choice) && (choice == 'q' || choice == 'Q')) { // Check for quit signal
            return;
        }
        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
        std::cout << "(!) Wrong input.\n";
    }
}

template <typename T>
std::pair<T, StdinAdjuster::ReturnStatus>
StdinAdjuster::choiceRetriever(std::vector<T> choices, const T & badValue) {

    // Retrieve input
    unsigned int choice;
    std::cin >> choice;

    // Validate input
    if (checkInputForFail()) {
        return std::make_pair(badValue, StdinAdjuster::ReturnStatus::BAD);
    }

    if (choice == 0) {
        return std::make_pair(badValue, StdinAdjuster::ReturnStatus::QUIT);
    }

    choice--;
    if (choice >= choices.size()) {
        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
        std::cout << "(!) Unknown choice.\n";
        return std::make_pair(badValue, StdinAdjuster::ReturnStatus::BAD);
    }

    return std::make_pair(choices[choice], StdinAdjuster::ReturnStatus::GOOD);
}

std::pair<std::string, StdinAdjuster::ReturnStatus>
StdinAdjuster::retrieveCourse(Semester & semester) {
    // Get all existing courses from semester's schedules
    std::set<std::string> uniqueCourses;
    for (auto & schedule : semester.schedulePtrs) {
        uniqueCourses.insert(schedule->course);
    }
    std::vector<std::string> courses(uniqueCourses.begin(), uniqueCourses.end());

    // Print header
    std::cout << "\nChoose a course to adjust:\n";
    for (size_t i = 0; i < courses.size(); i++) {
        std::cout << "\t" << i + 1 << ") " << courses[i] << "\n";
    }
    std::cout << "or '0' to exit.\n";
    std::cout << std::string(STDIN_ADJUSTER_SEPARATOR_LENGTH, '_') << std::endl;

    // Select
    return choiceRetriever<std::string>(courses, std::string());
}

std::pair<std::shared_ptr<Schedule>, StdinAdjuster::ReturnStatus>
StdinAdjuster::retrieveSchedule(Semester & semester, const std::string & course) {
    // Group schedules based on their course
    std::map<std::string, std::vector<std::shared_ptr<Schedule>>> groupedSchedules;
    for (auto & schedule : semester.schedulePtrs) {
        groupedSchedules[schedule->course].push_back(schedule);
    }

    // Retrieve course schedules
    if (!groupedSchedules.contains(course)) {
        return std::make_pair(nullptr, StdinAdjuster::ReturnStatus::BAD);
    }
    std::vector<std::shared_ptr<Schedule>> schedules = groupedSchedules[course];

    // Print header
    std::cout << "\nChoose a schedule to adjust:\n";
    for (size_t i = 0; i < schedules.size(); i++) {
        std::cout << "\t" << i + 1 << ") " << schedules[i]->name << "\n";
    }
    std::cout << "or '0' to exit.\n";
    std::cout << std::string(STDIN_ADJUSTER_SEPARATOR_LENGTH, '_') << std::endl;

    // Select
    return choiceRetriever<std::shared_ptr<Schedule>>(schedules, nullptr);
}

std::pair<std::shared_ptr<Entry>, StdinAdjuster::ReturnStatus>
StdinAdjuster::retrieveEntry(std::shared_ptr<Schedule> & schedule) {
    auto entries = schedule->entriesPtrs;
    // Print header
    std::cout << "Choose a entry to adjust:\n";
    for (size_t i = 0; i < entries.size(); i++) {
        std::cout << "\t" << i + 1 << ") " << entries[i]->legibleIdentifier << "\n";
    }
    std::cout << "or '0' to exit.\n";
    std::cout << std::string(STDIN_ADJUSTER_SEPARATOR_LENGTH, '_') << std::endl;

    // Select
    return choiceRetriever<std::shared_ptr<Entry>>(entries, nullptr);
}

void StdinAdjuster::adjustBonuses(Semester & semester) {
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    while (true) {
        // Print header
        std::cout << "Specify entries that should be prioritized or deprioritized in the timetable generation:\n";

        // Select course
        std::pair<std::string, StdinAdjuster::ReturnStatus> course = retrieveCourse(semester);
        if (course.second == StdinAdjuster::ReturnStatus::QUIT) {
            break;
        } else if (course.second == StdinAdjuster::ReturnStatus::BAD) {
            continue;
        }

        // Select schedule
        std::pair<std::shared_ptr<Schedule>, StdinAdjuster::ReturnStatus> schedule = retrieveSchedule(semester, course.first);
        if (schedule.second != StdinAdjuster::ReturnStatus::GOOD) {
            continue;
        }

        // Select entry
        std::pair<std::shared_ptr<Entry>, StdinAdjuster::ReturnStatus> entry = retrieveEntry(schedule.first);
        if (entry.second != StdinAdjuster::ReturnStatus::GOOD) {
            continue;
        }

        // Adjust bonus value
        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
        while (true) {
            // Print header
            std::cout << "Enter bonus (or malus) score in range -10 to +10 for a given entry." << "\n\n";
            std::cout << "Current value: [" << entry.first->getBonus() << "]\n";
            std::cout << "Enter new value or 'q' to exit: \n";
            std::cout << std::string(STDIN_ADJUSTER_SEPARATOR_LENGTH, '_') << std::endl;

            // Retrieve input
            std::string newValueString;
            std::cin >> newValueString;

            if (checkInputForFail()) {
                continue;
            }

            trim(newValueString);
            std::istringstream inputstringstream(newValueString);

            double value;
            char choice;

            if (inputstringstream >> value) { // Check for required value and configure bonus to it
                entry.first->setBonus(value);
                std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
                continue;
            }

            inputstringstream.clear();
            if ((inputstringstream >> choice) && (choice == 'q' || choice == 'Q')) { // Check for quit signal
                std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
                break;
            }
            std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
            std::cout << "(!) Wrong input.\n";
        }
    }
}

void StdinAdjuster::adjustIgnored(Semester & semester) {
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    while (true) {
        // Print header
        std::cout << "Specify schedules that should not be considered in the timetable generation.\n";

        // Select course
        std::pair<std::string, StdinAdjuster::ReturnStatus> course = retrieveCourse(semester);
        if (course.second == StdinAdjuster::ReturnStatus::QUIT) {
            break;
        } else if (course.second == StdinAdjuster::ReturnStatus::BAD) {
            continue;
        }

        // Select schedule
        std::pair<std::shared_ptr<Schedule>, StdinAdjuster::ReturnStatus> schedule = retrieveSchedule(semester, course.first);
        if (schedule.second != StdinAdjuster::ReturnStatus::GOOD) {
            continue;
        }

        // Adjust ignored
        adjustBool("Set whether this schedule will be ignored when generating timetable. (It will still be included in the generated timetable, but will be ignored for detecting collisions, continuation and bonuses.)",
            schedule.first->ignored);
    }
}

Priorities StdinAdjuster::operator()(Semester & semester) {
    Priorities result;

    bool active = true;
    while (active) {
        // Print choices
        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
        char choice;
        std::cout << "Choose a setting to adjust:\n";
        std::cout << "\t1) Keep entries continuous in a day.\n";
        std::cout << "\t2) Keep entries continuous in a week.\n";
        std::cout << "\t3) Maximum preferred consecutive hours.\n";
        std::cout << "\t4) Maximum interval such that entries count as consecutive.\n";
        std::cout << "\t5) Earliest preferred start hour in a day.\n";
        std::cout << "\t6) Latest preferred start hour in a day.\n";
        std::cout << "\t7) Ignore scheduling for entries.\n";
        std::cout << "\t8) Set priority of specific entries.\n";
        std::cout << "or 'q' to exit.\n";

        // Retrieve choice
        std::cout << std::string(STDIN_ADJUSTER_SEPARATOR_LENGTH, '=') << std::endl;
        std::cin >> choice;

        // Handle choice
        switch (choice) {
            case '1':
                adjustBool("All entries within a single day are preferred to be scheduled back-to-back with the least time gaps between them.",
                    result.keepCoherentInDay);
                continue;
            case '2':
                adjustBool("All scheduled days are preferred to be scheduled back-to-back without empty days between them.",
                    result.keepCoherentInWeek);
                continue;
            case '3':
                adjustHour("Maximum amount of hours that are preferred to be scheduled consecutively back-to-back.",
                    result.penaliseManyConsecutiveHours);
                continue;
            case '4':
                adjustValue<unsigned int>("Maximum amount of minutes that can pass between two entries for them to still be considered consecutive.",
                    result.minutesToBeConsecutive);
                continue;
            case '5':
                adjustHour("Earliest preferred hour of day after which schedule should ideally should begin.",
                    result.penaliseBeforeHour);
                continue;
            case '6':
                adjustHour("Latest preferred hour of day after which ideally no entries should begin.",
                    result.penaliseAfterHour);
                continue;
            case '7':
                adjustIgnored(semester);
                continue;
            case '8':
                adjustBonuses(semester);
                continue;
            case 'q':
                active = false;
                break;
            default:
                continue;
        }
    }

    return result;
}
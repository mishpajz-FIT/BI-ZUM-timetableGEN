#ifndef TIMETABLE_H
#define TIMETABLE_H

#include "timeinterval.h"

#include <cstdint>
#include <vector>
#include <map>
#include <string>

struct Entry {
    uint32_t id;
    std::vector<TimeInterval> timeslots;
};

template <typename T>
using dictionary = std::map<std::string, T>;

class Course {
    std::string code;
    dictionary<std::vector<Entry>> entries;
};

class Timetable {
    dictionary<dictionary<uint16_t>> schedule;
};

#endif /* TIMETABLE_H */
#ifndef SUBJECTS_H
#define SUBJECTS_H

#include "Data/timeinterval.h"

#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <tuple>

struct Schedule {
    uint32_t id;
    std::vector<TimeInterval> timeslots;
};

struct Entry {
    uint32_t id;
    std::vector<Schedule> schedules;
};

struct Course {
    std::string name;
    std::vector<Entry> entries;
};

#endif /* SUBJECTS_H */
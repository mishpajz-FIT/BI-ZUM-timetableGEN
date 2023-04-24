#ifndef SUBJECTS_H
#define SUBJECTS_H

#include "timeinterval.h"

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
    std::vector<Entry> schedules;
};

struct Course {
    std::string code;
    std::vector<Entry> entries;
};

#endif /* SUBJECTS_H */
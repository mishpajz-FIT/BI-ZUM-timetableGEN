#ifndef SUBJECTS_H
#define SUBJECTS_H

#include "timeinterval.h"

#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <tuple>

struct Entry {
    uint32_t id;
    std::vector<TimeInterval> timeslots;
};

struct Class {
    uint32_t id;
    std::vector<Entry> entries;
};

struct Course {
    std::string code;
    std::vector<Class> classes;
};

#endif /* SUBJECTS_H */
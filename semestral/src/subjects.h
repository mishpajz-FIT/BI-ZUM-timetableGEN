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

template <typename T>
using dictionary = std::map<std::string, T>;

class Subject {
    std::string code;
    dictionary<std::vector<Entry>> entries;
};

#endif /* SUBJECTS_H */
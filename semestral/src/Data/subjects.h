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
    std::string additionalInformation;
    std::vector<TimeInterval> timeslots;
};

struct Course {
    std::string name;
    std::map<std::string, Schedule> schedules;
};

#endif /* SUBJECTS_H */
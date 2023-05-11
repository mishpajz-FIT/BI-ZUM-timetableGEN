#ifndef SUBJECTS_H
#define SUBJECTS_H

#include "Data/timeinterval.h"

#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <memory>
#include <set>

struct Schedule;
struct Course;

struct Entry {

private:
    struct CompareWeakPtr {
        bool operator()(const std::weak_ptr<Entry> & lhs, const std::weak_ptr<Entry> & rhs) const;
    };

public:
    uint32_t id;
    std::string additionalInformation;
    std::vector<TimeInterval> timeslots;

    std::set<std::weak_ptr<Entry>, CompareWeakPtr> collisions;

    std::weak_ptr<Schedule> schedule;
    size_t indexInSchedule;

    Entry(size_t ix, const std::weak_ptr<Schedule> & parent, uint32_t id = 0);

};

struct Schedule {
    std::string name;
    std::vector<std::shared_ptr<Entry>> entriesPtrs;

    std::weak_ptr<Course> course;
    Schedule(const std::weak_ptr<Course> & parent);
    Schedule(const std::weak_ptr<Course> & parent, const std::string & name);

};

struct Course {
    std::string name;
    std::map <std::string, std::shared_ptr<Schedule>> schedulesPtrs;
};

struct Semester {
    std::vector <std::shared_ptr<Course>> coursesPtrs;
};

#endif /* SUBJECTS_H */
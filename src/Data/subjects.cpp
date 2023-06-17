#include "Data/subjects.h"


Entry::Entry(size_t ix, const std::weak_ptr<Schedule> & parent, uint32_t id) :
    bonus(0),
    id(id),
    additionalInformation(),
    timeslots(),
    schedule(parent),
    indexInSchedule(ix) { }

double Entry::getBonus() const {
    return bonus;
}

void Entry::setBonus(double value) {
    if (value > 10) {
        bonus = 10;
    } else if (value < -10) {
        bonus = -10;
    } else {
        bonus = value;
    }
}

Schedule::Schedule(const std::weak_ptr<Course> & parent) : name(), course(parent), ignored(false) { }

Schedule::Schedule(const std::weak_ptr<Course> & parent, const std::string & name) : name(name), course(parent), ignored(false) { }

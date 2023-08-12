#include "Data/subjects.h"

Entry::Entry() :
    bonus(0),
    legibleIdentifier(),
    additionalInformation(),
    timeslots(),
    schedule(),
    indexInSchedule(0) { }

Entry::Entry(size_t ix, const std::weak_ptr<Schedule> & parent) :
    bonus(0),
    legibleIdentifier(),
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

Schedule::Schedule() : name(), ignored(false) { }

Schedule::Schedule(const std::string & name) : name(name), ignored(false) { }

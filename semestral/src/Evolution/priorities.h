#ifndef PRIORITIES_H
#define PRIORITIES_H

#include <cstdint>
#include <algorithm>

struct Priorities {

    bool keepCoherentInDay;
    bool keepCoherentInWeek;

    uint8_t penaliseBeforeHour;

};

struct Scores {
    double coherentInDay;
    double coherentInWeek;
    double collisions;
    double earlyHours;
    double bonuses;

    Scores();

    void setToMinValuesFrom(const Scores & s);

    void setToMaxValuesFrom(const Scores & s);

};

#endif /* PRIORITIES_H */
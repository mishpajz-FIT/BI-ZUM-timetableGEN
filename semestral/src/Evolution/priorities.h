#ifndef PRIORITIES_H
#define PRIORITIES_H

#include <cstdint>
#include <algorithm>

struct Priorities {

    bool keepCoherentInDay;
    bool keepCoherentInWeek;

    uint8_t penaliseBeforeHour;
    uint8_t penaliseHoursTogether;
    uint8_t penaliseAfterHour;

    unsigned int minutesToBeConsecutive;

    Priorities();

};

struct Scores {
    double coherentInDay;
    double coherentInWeek;
    double collisions;
    double wrongHours;
    double bonuses;

    Scores();

    void setToMinValuesFrom(const Scores & s);

    void setToMaxValuesFrom(const Scores & s);

};

#endif /* PRIORITIES_H */
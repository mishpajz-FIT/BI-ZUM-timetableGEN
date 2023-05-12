#ifndef PRIORITIES_H
#define PRIORITIES_H

#include <cstdint>

struct Priorities {

    bool keepCoherentInDay;
    bool keepCoherentInWeek;

    uint8_t penaliseBeforeHour;

};

#endif /* PRIORITIES_H */
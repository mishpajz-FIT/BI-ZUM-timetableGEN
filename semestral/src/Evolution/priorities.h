#ifndef PRIORITIES_H
#define PRIORITIES_H

#include <cstdint>

struct Priorities {
    bool keepTogether;

    bool enabledPenaliseBeforeTime;
    uint8_t penaliseBeforeTime;

};

#endif /* PRIORITIES_H */
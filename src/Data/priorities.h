/**
 * @file priorities.h
 * @author Michal Dobes
 * @date 2023-08-13
 *
 * @brief Data structures describing priorities for timetable generation
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef PRIORITIES_H
#define PRIORITIES_H

#include "Data/subjects.h"

#include <cstdint>
#include <algorithm>
#include <exception>

/**
 * @brief Representation of priorities for generating a timetable
 *
 */
struct Priorities {

    bool keepCoherentInDay;
    bool keepCoherentInWeek;

    uint8_t penaliseBeforeHour; //<! If disabled set to zero
    uint8_t penaliseManyConsecutiveHours; //<! If desabled set to zero
    uint8_t penaliseAfterHour; //<! If disabled set to zero

    unsigned int minutesToBeConsecutive; //!< Minutes needed for intervals to be consecutive (default 30)

    Priorities();

};

/**
 * @brief Interval and it's Entry
 *
 * Each entry can have multiple intervals, this way each interval is its own element
 * with link to entry it comes from.
 *
 * @see Entry
 *
 */
using IntervalEntry = std::pair<TimeInterval, std::shared_ptr<Entry>>;

#endif /* PRIORITIES_H */
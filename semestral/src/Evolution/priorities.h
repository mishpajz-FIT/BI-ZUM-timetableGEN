/**
 * @file priorities.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Data structures describing priorities and scores for timetable generation
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

    uint8_t penaliseBeforeHour; // If disabled set to zero
    uint8_t penaliseManyConsecutiveHours; // If desabled set to zero
    uint8_t penaliseAfterHour; // If disabled set to zero

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

/**
 * @brief Scores for selected Entries
 *
 * Fitness value is calculated based on these scores.
 *
 */
struct Scores {
    double coherentInDay; //!< Amount of incoherent parts in days
    double coherentInWeek; //!< Length of week (from first entry to last)
    double collisions; //!< Amount of collisions
    double manyConsecutiveHours; //!< Number of minutes over the preferred limit of consecutive hours
    double wrongStartTimes; //!< Minutes for start times after or before preferred limits
    double bonuses; //!< Sum of bonuses in selected Entries

    Scores();

    /**
     * @brief Sets the scores to minimum value from another score
     *
     * Values are minimums from this and another score object.
     *
     * @param s another score
     */
    void setToMinValuesFrom(const Scores & s);

    /**
     * @brief Sets the scores to maximum value from another score
     *
     * Values are maximums from this and another score object.
     *
     * @param s another score
     */
    void setToMaxValuesFrom(const Scores & s);

    /**
     * @brief Calculate all scores for selected Entries
     *
     * Supplied intervals need to be sorted by start time.
     *
     * @param sortedIntervals intervals and it's entry
     * @param p priorities for timetable generation
     */
    void calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    /**
     * @brief Calculate fitness out of scores
     *
     * Requires minimum and maximum scores reached in this generation
     *
     * @param minValues minimum scores reached in this generation
     * @param maxValues maximum scores reached in this generation
     * @param p priorities for timetable generation
     * @return double fitness
     */
    double convertScoreToFitness(const Scores & minValues, const Scores & maxValues, const Priorities & p) const;

private:

    /**
     * @brief Map values to inverse 0-10 scale
     *
     * Values are mapped in inverse to 0-10 scale, with the spacing between them kept proportionate.
     *
     * Largest value is mapped to smallest (0), smallest value is mapped to largest (10).
     *
     * @throws std::invalid_argument value is larger than max or smaller than min
     *
     * @param value value to map
     * @param min minimum value
     * @param max maximum value
     * @return double mapped value
     */
    inline double inverseScoring(double value, double min, double max) const;

    /**
     * @brief Calculate 'coherentInDay' on this score
     *
     * Supplied intervals need to be sorted by start time.
     *
     * @see Scores.coherentInDay
     *
     * @param sortedIntervals intervals and it's entry
     * @param p priorities for timetable generation
     */
    void calculateCoherentInDayScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    /**
     * @brief Calculate 'coherentInWeek' on this score
     *
     * Supplied intervals need to be sorted by start time.
     *
     * @see Scores.coherentInWeek
     *
     * @param sortedIntervals intervals and it's entry
     * @param p priorities for timetable generation
     */
    void calculateCoherentInWeekScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    /**
     * @brief Calculate 'collisions' on this score
     *
     * Supplied intervals need to be sorted by start time.
     *
     * @see Scores.collisions
     *
     * @param sortedIntervals intervals and it's entry
     * @param p priorities for timetable generation
     */
    void calculateCollisionsScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    /**
     * @brief Calculate 'manyConsecutiveHours' on this score
     *
     * Supplied intervals need to be sorted by start time.
     *
     * @see Scores.manyConsecutiveHours
     *
     * @param sortedIntervals intervals and it's entry
     * @param p priorities for timetable generation
     */
    void calculateManyConsecutiveHoursScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    /**
     * @brief Calculate 'wrongStartTimes' on this score
     *
     * Supplied intervals need to be sorted by start time.
     *
     * @see Scores.wrongStartTimes
     *
     * @param sortedIntervals intervals and it's entry
     * @param p priorities for timetable generation
     */
    void calculateWrongStartTimesScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    /**
     * @brief Calculate 'bonuses' on this score
     *
     * Supplied intervals need to be sorted by start time.
     *
     * @see Scores.bonuses
     *
     * @param sortedIntervals intervals and it's entry
     * @param p priorities for timetable generation
     */
    void calculateBonusesScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

};

#endif /* PRIORITIES_H */
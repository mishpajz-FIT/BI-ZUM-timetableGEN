#ifndef PRIORITIES_H
#define PRIORITIES_H

#include "Data/subjects.h"

#include <cstdint>
#include <algorithm>

struct Priorities {

    bool keepCoherentInDay;
    bool keepCoherentInWeek;

    uint8_t penaliseBeforeHour;
    uint8_t penaliseManyConsecutiveHours;
    uint8_t penaliseAfterHour;

    unsigned int minutesToBeConsecutive;

    Priorities();

};

using IntervalEntry = std::pair<TimeInterval, std::shared_ptr<Entry>>;

struct Scores {
    double coherentInDay;
    double coherentInWeek;
    double collisions;
    double manyConsecutiveHours;
    double wrongStartTimes;
    double bonuses;

    Scores();

    void setToMinValuesFrom(const Scores & s);

    void setToMaxValuesFrom(const Scores & s);

    void calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    double convertScoreToFitness(const Scores & minValues, const Scores & maxValues) const;

private:

    void calculateCoherentInDayScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    void calculateCoherentInWeekScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    void calculateCollisionsScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    void calculateManyConsecutiveHoursScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    void calculateWrongStartTimesScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    void calculateBonusesScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

};

#endif /* PRIORITIES_H */
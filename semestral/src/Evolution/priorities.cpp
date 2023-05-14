#include "Evolution/priorities.h"

Priorities::Priorities() :
    keepCoherentInDay(true),
    keepCoherentInWeek(true),
    penaliseBeforeHour(0),
    penaliseManyConsecutiveHours(0),
    penaliseAfterHour(0),
    minutesToBeConsecutive(30) { }

Scores::Scores() :
    coherentInDay(0),
    coherentInWeek(0),
    collisions(0),
    manyConsecutiveHours(0),
    wrongStartTimes(0),
    bonuses(0) { }

void Scores::setToMinValuesFrom(const Scores & s) {

#define minimum(val) val = std::min(val, s.val)

    minimum(coherentInDay);
    minimum(coherentInWeek);
    minimum(collisions);
    minimum(manyConsecutiveHours);
    minimum(wrongStartTimes);
    minimum(bonuses);

}

void Scores::setToMaxValuesFrom(const Scores & s) {

#define maximum(val) val = std::max(val, s.val)

    maximum(coherentInDay);
    maximum(coherentInWeek);
    maximum(collisions);
    maximum(manyConsecutiveHours);
    maximum(wrongStartTimes);
    maximum(bonuses);

}

void Scores::calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {

    coherentInDay = 0;
    coherentInWeek = 0;
    collisions = 0;
    manyConsecutiveHours = 0;
    wrongStartTimes = 0;
    bonuses = 0;

    calculateCoherentInDayScore(sortedIntervals, p);
    calculateCoherentInWeekScore(sortedIntervals, p);
    calculateCollisionsScore(sortedIntervals, p);
    calculateManyConsecutiveHoursScore(sortedIntervals, p);
    calculateWrongStartTimesScore(sortedIntervals, p);
    calculateBonusesScore(sortedIntervals, p);
}

#define SCORE_CALCULATION_COLLISION_MULTIPLIER 0.6
#define SCORE_CALCULATION_COHERENTDAY_MULTIPLIER 0.2
#define SCORE_CALCULATION_COHERENTWEEK_MULTIPLIER 0.2
#define SCORE_CALCULATION_WRONGSTARTTIME_MULTIPLIER 0.13
#define SCORE_CALCULATION_CONSECUTIVEHOURS_MULTIPLIER 0.07
#define SCORE_CALCULATION_BONUS_MULTIPLIER 0.2


double Scores::convertScoreToFitness(const Scores & minValues, const Scores & maxValues, const Priorities & p) const {
    double result = 0;

#define convertScoring(val) inverseScoring(val, minValues.val, maxValues.val)
    result += convertScoring(collisions) * SCORE_CALCULATION_COLLISION_MULTIPLIER;
    if (p.keepCoherentInDay) {
        result += convertScoring(coherentInDay) * SCORE_CALCULATION_COHERENTDAY_MULTIPLIER;
    }
    if (p.keepCoherentInWeek) {
        result += convertScoring(coherentInWeek) * SCORE_CALCULATION_COHERENTWEEK_MULTIPLIER;
    }
    if (p.penaliseAfterHour != 0 || p.penaliseBeforeHour != 0) {
        result += convertScoring(wrongStartTimes) * SCORE_CALCULATION_WRONGSTARTTIME_MULTIPLIER;
    }
    if (p.penaliseManyConsecutiveHours != 0) {
        result += convertScoring(manyConsecutiveHours) * SCORE_CALCULATION_CONSECUTIVEHOURS_MULTIPLIER;
    }
    result += bonuses * SCORE_CALCULATION_BONUS_MULTIPLIER;
    return result;
}


double Scores::inverseScoring(double value, double min, double max) const {

    if (value < min || value > max) {
        printf("val=%lf, min:%lf max:%lf\n", value, min, max);
        throw std::invalid_argument("Value is out of min-max range.");
    }

    if (min == max) {
        return 10;
    }

    double factor = 10.0 / (max - min);
    double transformedValue = max - value;

    return transformedValue * factor;
}

void Scores::calculateCoherentInDayScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    if (!p.keepCoherentInDay) {
        return;
    }
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {
        if (it->second->schedule.lock()->ignored) {
            continue;
        }

        auto next = it + 1;
        while (next != sortedIntervals.end() && it->first.day == next->first.day) {
            if (next->second->schedule.lock()->ignored) {
                next++;
                continue;
            }

            if (next->first.startTime < it->first.endTime) {
                break;
            }

            size_t coherenceValue = (next->first.startTime.valueInMinutes() - it->first.endTime.valueInMinutes());
            if (coherenceValue >= p.minutesToBeConsecutive) {
                coherentInDay++;
            }
            break;
        }
    }
}

void Scores::calculateCoherentInWeekScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    auto beginIt = sortedIntervals.begin();
    while (beginIt != sortedIntervals.end()) {
        if (!beginIt->second->schedule.lock()->ignored) {
            break;
        }
        beginIt++;
    }

    if (beginIt == sortedIntervals.end()) {
        return;
    }

    auto endIt = sortedIntervals.rbegin();
    while (endIt != sortedIntervals.rend()) {
        if (!endIt->second->schedule.lock()->ignored) {
            break;
        }
        endIt++;
    }

    if (endIt == sortedIntervals.rend()) {
        return;
    }

    coherentInWeek = static_cast<size_t>(endIt->first.day) - static_cast<size_t>(beginIt->first.day);
}

void Scores::calculateCollisionsScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {

        if (it->second->schedule.lock()->ignored) {
            continue;
        }

        auto collisionIt = it + 1;
        while (collisionIt != sortedIntervals.end()
            && ((collisionIt->first.startTime < it->first.endTime) && (it->first.day == collisionIt->first.day))) {

            if (it->first.parity == collisionIt->first.parity
                || (it->first.parity == TimeInterval::Parity::Both || collisionIt->first.parity == TimeInterval::Parity::Both)) {

                if (!collisionIt->second->schedule.lock()->ignored) {
                    collisions++;
                }
            }

            collisionIt++;
        }
    }
}

void Scores::calculateManyConsecutiveHoursScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    if (p.penaliseManyConsecutiveHours == 0 || sortedIntervals.size() == 0) {
        return;
    }

    TimeInterval currentInterval = sortedIntervals.begin()->first;

    auto it = sortedIntervals.begin();
    while (it != sortedIntervals.end()) {

        if (it->second->schedule.lock()->ignored) {
            it++;
            continue;
        }

        size_t difference = (it->first.startTime.valueInMinutes() - currentInterval.endTime.valueInMinutes());
        if (currentInterval.day != it->first.day || difference > p.minutesToBeConsecutive) {
            size_t length = (currentInterval.endTime.valueInMinutes() - currentInterval.startTime.valueInMinutes());
            size_t hours = length / 60;
            if (hours > p.penaliseManyConsecutiveHours) {
                manyConsecutiveHours += (length - p.penaliseManyConsecutiveHours * 60);
            }

            currentInterval = it->first;
        } else {
            currentInterval.endTime = it->first.endTime;
        }

        it++;
    }

    size_t length = (currentInterval.endTime.valueInMinutes() - currentInterval.startTime.valueInMinutes());
    size_t hours = length / 60;
    if (hours > p.penaliseManyConsecutiveHours) {
        manyConsecutiveHours += (length - p.penaliseManyConsecutiveHours * 60);
    }
}

#define SCORE_CALCULATION_WRONGSTARTTIMEDEFAULT 60

void Scores::calculateWrongStartTimesScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {

    if (p.penaliseBeforeHour != 0) {
        for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {

            if (it->second->schedule.lock()->ignored) {
                continue;
            }

            TimeInterval::TimeStamp penalisationTime(p.penaliseBeforeHour, 0);
            if (it->first.startTime.valueInMinutes() <= penalisationTime.valueInMinutes()) {
                wrongStartTimes += SCORE_CALCULATION_WRONGSTARTTIMEDEFAULT + (penalisationTime.valueInMinutes() - it->first.startTime.valueInMinutes());
            }
        }
    }

    if (p.penaliseAfterHour != 0) {
        for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {

            if (it->second->schedule.lock()->ignored) {
                continue;
            }

            TimeInterval::TimeStamp penalisationTime(p.penaliseAfterHour, 0);
            if (it->first.startTime.valueInMinutes() >= penalisationTime.valueInMinutes()) {
                wrongStartTimes += SCORE_CALCULATION_WRONGSTARTTIMEDEFAULT + (it->first.startTime.valueInMinutes() - penalisationTime.valueInMinutes());
            }
        }
    }
}

void  Scores::calculateBonusesScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {
        bonuses += it->second->getBonus();
    }
}
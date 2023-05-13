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

#define minimum(val) val = std::min(val, s.val);

    minimum(coherentInDay);
    minimum(coherentInWeek);
    minimum(collisions);
    minimum(manyConsecutiveHours);
    minimum(wrongStartTimes);
    minimum(bonuses);

}

void Scores::setToMaxValuesFrom(const Scores & s) {

#define maximum(val) val = std::max(val, s.val);

    maximum(coherentInDay);
    maximum(coherentInWeek);
    maximum(collisions);
    minimum(manyConsecutiveHours);
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

double Scores::convertScoreToFitness(const Scores & minValues, const Scores & maxValues) const {
    double result = 0;

#define inverseScoring(val) (10 - 9 * ((val - minValues.val) / (maxValues.val - minValues.val)))

    result += 5 * (inverseScoring(collisions));
    result += inverseScoring(coherentInDay);
    /* result += (inverseScoring(wrongStartTimes));
    result += inverseScoring(coherentInWeek);
    result += bonuses;*/

    return result;
}

void Scores::calculateCoherentInDayScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    if (!p.keepCoherentInDay) {
        return;
    }
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {
        /*if (it->second->schedule.lock()->ignored) {
            continue;
        }*/

        auto next = it + 1;
        while (next != sortedIntervals.end() && it->first.day == next->first.day) {
            /*if (next->second->schedule.lock()->ignored) {
                next++;
                continue;
            }*/

            size_t coherenceValue = (next->first.startTime.valueInMinutes() - it->first.endTime.valueInMinutes());
            if (coherenceValue >= p.minutesToBeConsecutive) {
                coherentInDay += coherenceValue;
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

    coherentInWeek = static_cast<size_t>(beginIt->first.day) - static_cast<size_t>(endIt->first.day);
}

void Scores::calculateCollisionsScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {

        /*if (it->second->schedule.lock()->ignored) {
            continue;
        }*/

        // Collision score
        auto collisionIt = it + 1;
        while (collisionIt != sortedIntervals.end()
            && ((collisionIt->first.startTime < it->first.endTime) && (it->first.day == collisionIt->first.day))) {

            if (it->first.parity == collisionIt->first.parity
                || (it->first.parity == TimeInterval::Parity::Both || collisionIt->first.parity == TimeInterval::Parity::Both)) {

                //if (!collisionIt->second->schedule.lock()->ignored) {
                collisions++;
            //}
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
                manyConsecutiveHours += hours;
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
        manyConsecutiveHours += hours;
    }
}

void Scores::calculateWrongStartTimesScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {

    if (p.penaliseBeforeHour != 0) {
        for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {

            if (it->second->schedule.lock()->ignored) {
                continue;
            }

            if (it->first.startTime.hour < p.penaliseBeforeHour) {
                wrongStartTimes++;
            }
        }
    }

    if (p.penaliseAfterHour != 0) {
        for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {

            if (it->second->schedule.lock()->ignored) {
                continue;
            }

            if (it->first.startTime.hour > p.penaliseAfterHour) {
                wrongStartTimes++;
            }
        }
    }
}

void  Scores::calculateBonusesScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {
        bonuses += it->second->getBonus();
    }
}
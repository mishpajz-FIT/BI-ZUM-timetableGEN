#include "scores.h"

Scores::Scores(const Priorities & p) : priorities(p), scores() {

    scores.emplace("collisions", new CollisionsScore());

    scores.emplace("bonuses", new BonusesScore());

    if (p.keepCoherentInDay) {
        scores.emplace("coherentInDay", new CoherentInDayScore());
    }

    if (p.keepCoherentInWeek) {
        scores.emplace("coherentInWeek", new CoherentInWeekScore());
    }

    if (p.penaliseAfterHour != 0 || p.penaliseBeforeHour != 0) {
        scores.emplace("wrongStartTime", new WrongStartTimesScore());
    }

    if (p.penaliseManyConsecutiveHours != 0) {
        scores.emplace("manyConsecutiveHours", new ManyConsecutiveHoursScore());
    }
}

Scores::~Scores() {
    for (auto & score : scores) {
        score.second.reset();
    }
}

Scores::Scores(const Scores & s) : priorities(s.priorities), scores() {
    for (auto & score : s.scores) {
        scores.emplace(score.first, score.second->clone());
    }
}

Scores & Scores::operator=(const Scores & s) {
    priorities = s.priorities;

    for (auto & score : scores) {
        score.second.reset();
    }

    scores.clear();

    for (auto & score : s.scores) {
        scores.emplace(score.first, score.second->clone());
    }

    return *this;
}

Scores & Scores::setToMinValuesFrom(const Scores & s) {

    for (auto & score : scores) {

        auto retrievedScore = s.scores.at(score.first);

        score.second->value = std::min(score.second->value, retrievedScore->value);
    }

    return *this;
}

Scores & Scores::setToMaxValuesFrom(const Scores & s) {

    for (auto & score : scores) {

        auto retrievedScore = s.scores.at(score.first);

        score.second->value = std::max(score.second->value, retrievedScore->value);
    }

    return *this;
}

void Scores::calculateScore(std::vector<IntervalEntry> & intervals) {

    // Sort intervals by start time
    std::sort(intervals.begin(), intervals.end(), [ ] (const IntervalEntry & lhs, const IntervalEntry & rhs) -> bool {
        return lhs.first < rhs.first;
        });


    for (auto & score : scores) {
        score.second->calculateScore(intervals, priorities);
    }
}

double Scores::convertScoreToFitness(const Scores & minValues, const Scores & maxValues) const {
    double result = 0;

    for (auto & score : scores) {

        auto min = minValues.scores.at(score.first);
        auto max = maxValues.scores.at(score.first);

        result += score.second->getWeight() * inverseScoring(score.second->value, min->value, max->value);
    }

    return result;
}

double Scores::inverseScoring(double value, double min, double max) const {

    if (value < min || value > max) {
        throw std::invalid_argument("Value is out of min-max range.");
    }

    if (min == max) {
        return 10;
    }

    double factor = 10.0 / (max - min);
    double transformedValue = max - value;

    return transformedValue * factor;
}

Score::Score() : value(0) { }

void Score::calculateScore(std::vector<IntervalEntry> & intervals, const Priorities & p) {
    value = 0;
}

double CollisionsScore::getWeight() const {
    return 0.6;
}

double CoherentInDayScore::getWeight() const {
    return 0.2;
}

double CoherentInWeekScore::getWeight() const {
    return 0.2;
}

double ManyConsecutiveHoursScore::getWeight() const {
    return 0.07;
}

double WrongStartTimesScore::getWeight() const {
    return 0.13;
}

double BonusesScore::getWeight() const {
    return 0.2;
}

void CollisionsScore::calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    Score::calculateScore(sortedIntervals, p);

    // Iterate through all intervals
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {

        if (it->second->schedule.lock()->ignored) { // Skip ignored intervals
            continue;
        }

        // Iterate through all intervals that follow this interval and could collide with this interval
        auto collisionIt = it + 1;
        while (collisionIt != sortedIntervals.end()
            && ((collisionIt->first.startTime < it->first.endTime) && (it->first.day == collisionIt->first.day))) {

            if (collisionIt->second->schedule.lock()->ignored) { // Skip if the other interval is ignored
                collisionIt++;
                continue;
            }

            // Check if they collide by having same parity (or both parity), and raise collisions score
            if (it->first.parity == collisionIt->first.parity
                || (it->first.parity == TimeInterval::Parity::Both || collisionIt->first.parity == TimeInterval::Parity::Both)) {

                value++;
            }

            collisionIt++;
        }
    }
}

void CoherentInDayScore::calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    Score::calculateScore(sortedIntervals, p);

    // Iterate through all intervals
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {
        if (it->second->schedule.lock()->ignored) { // Skip ignored
            continue;
        }

        auto next = it + 1;
        while (next != sortedIntervals.end() && it->first.day == next->first.day) { // FInd next interval that is not ingnored
            if (next->second->schedule.lock()->ignored) {
                next++;
                continue;
            }

            // Check if start time of the next interval is larger than minutes to be consecutive 
            // (and if it is, raise the score)

            if (next->first.startTime < it->first.endTime) {
                break;
            }

            size_t coherenceValue = (next->first.startTime.valueInMinutes() - it->first.endTime.valueInMinutes());
            if (coherenceValue >= p.minutesToBeConsecutive) {
                value++;
            }
            break;
        }
    }
}

void CoherentInWeekScore::calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    Score::calculateScore(sortedIntervals, p);

    auto beginIt = sortedIntervals.begin();
    while (beginIt != sortedIntervals.end()) { // Find first interval that is not ignored
        if (!beginIt->second->schedule.lock()->ignored) {
            break;
        }
        beginIt++;
    }

    if (beginIt == sortedIntervals.end()) {
        return;
    }

    auto endIt = sortedIntervals.rbegin();
    while (endIt != sortedIntervals.rend()) { // Find last interval that is not ignored
        if (!endIt->second->schedule.lock()->ignored) {
            break;
        }
        endIt++;
    }

    if (endIt == sortedIntervals.rend()) {
        return;
    }

    // Calculate days between first and last interval
    value = static_cast<size_t>(endIt->first.day) - static_cast<size_t>(beginIt->first.day);
}

void ManyConsecutiveHoursScore::calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    Score::calculateScore(sortedIntervals, p);

    if (sortedIntervals.size() == 0) {
        return;
    }

    TimeInterval currentInterval = sortedIntervals.begin()->first; // Current interval is the current interval plus 
    // previous intervals that are continuous with this one
    auto it = sortedIntervals.begin();
    while (it != sortedIntervals.end()) { // Iterate through all intervals

        if (it->second->schedule.lock()->ignored) { // Skip over ignored intervals
            it++;
            continue;
        }

        // If interval in another day than current interval or if difference too big to be consecutive
        size_t difference = (it->first.startTime.valueInMinutes() - currentInterval.endTime.valueInMinutes());
        if (currentInterval.day != it->first.day || difference > p.minutesToBeConsecutive) {

            // If current interval has minimum length, add minutes over to many consecutive hours score
            size_t length = (currentInterval.endTime.valueInMinutes() - currentInterval.startTime.valueInMinutes());
            size_t hours = length / 60;
            if (hours > p.penaliseManyConsecutiveHours) {
                value += (length - p.penaliseManyConsecutiveHours * 60);
            }

            // Reset current interval
            currentInterval = it->first;
        } else { // Else extend current interval by this interval
            currentInterval.endTime = it->first.endTime;
        }

        it++;
    }

    // If current interval (last one) has minimum length, add minutes over to many consecutive hours score
    size_t length = (currentInterval.endTime.valueInMinutes() - currentInterval.startTime.valueInMinutes());
    size_t hours = length / 60;
    if (hours > p.penaliseManyConsecutiveHours) {
        value += (length - p.penaliseManyConsecutiveHours * 60);
    }
}

// Default score to add for any start time out of prefferred bounds
#define SCORE_CALCULATION_WRONGSTARTTIMEDEFAULT 60

void WrongStartTimesScore::calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    Score::calculateScore(sortedIntervals, p);

    // Iterate through all intervals
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {
        if (it->second->schedule.lock()->ignored) { // Skip over ignored intervals
            continue;
        }

        if (p.penaliseBeforeHour != 0) {
            // Check if interval starts before preferred bound and raise wrong start times score
            TimeInterval::TimeStamp penalisationBeforeTime(p.penaliseBeforeHour, 0);
            if (it->first.startTime.valueInMinutes() <= penalisationBeforeTime.valueInMinutes()) {
                value += SCORE_CALCULATION_WRONGSTARTTIMEDEFAULT + (penalisationBeforeTime.valueInMinutes() - it->first.startTime.valueInMinutes());
            }
        }
        if (p.penaliseAfterHour != 0) {
            // Check if interval starts after preferred bound and raise wrong start times score
            TimeInterval::TimeStamp penalisationAfterTime(p.penaliseAfterHour, 0);
            if (it->first.startTime.valueInMinutes() >= penalisationAfterTime.valueInMinutes()) {
                value += SCORE_CALCULATION_WRONGSTARTTIMEDEFAULT + (it->first.startTime.valueInMinutes() - penalisationAfterTime.valueInMinutes());
            }
        }
    }
}

void BonusesScore::calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) {
    Score::calculateScore(sortedIntervals, p);

    // Iterate through all intervals
    for (auto it = sortedIntervals.begin(); it != sortedIntervals.end(); it++) {

        if (it->second->schedule.lock()->ignored) { // Skip over ignored intervals
            continue;
        }

        value += it->second->getBonus();
    }
}

CollisionsScore * CollisionsScore::clone() const {
    return new CollisionsScore(*this);
}

CoherentInDayScore * CoherentInDayScore::clone() const {
    return new CoherentInDayScore(*this);
}

CoherentInWeekScore * CoherentInWeekScore::clone() const {
    return new CoherentInWeekScore(*this);
}

ManyConsecutiveHoursScore * ManyConsecutiveHoursScore::clone() const {
    return new ManyConsecutiveHoursScore(*this);
}

WrongStartTimesScore * WrongStartTimesScore::clone() const {
    return new WrongStartTimesScore(*this);
}

BonusesScore * BonusesScore::clone() const {
    return new BonusesScore(*this);
}
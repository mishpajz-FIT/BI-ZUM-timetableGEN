#include "Evolution/priorities.h"

Priorities::Priorities() :
    keepCoherentInDay(true),
    keepCoherentInWeek(true),
    penaliseBeforeHour(0),
    penaliseHoursTogether(0),
    penaliseAfterHour(0),
    minutesToBeConsecutive(30) { }

Scores::Scores() :
    coherentInDay(0),
    coherentInWeek(0),
    collisions(0),
    wrongHours(0),
    bonuses(0) { }

void Scores::setToMinValuesFrom(const Scores & s) {

#define minimum(val) val = std::min(val, s.val);

    minimum(coherentInDay);
    minimum(coherentInWeek);
    minimum(collisions);
    minimum(wrongHours);
    minimum(bonuses);

}

void Scores::setToMaxValuesFrom(const Scores & s) {

#define maximum(val) val = std::max(val, s.val);

    maximum(coherentInDay);
    maximum(coherentInWeek);
    maximum(collisions);
    maximum(wrongHours);
    maximum(bonuses);

}
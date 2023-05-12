#include "Evolution/priorities.h"

Scores::Scores():
    coherentInDay(0),
    coherentInWeek(0),
    collisions(0),
    earlyHours(0),
    bonuses(0) { }

void Scores::setToMinValuesFrom(const Scores & s) {

    #define minimum(val) val = std::min(val, s.val);

    minimum(coherentInDay);
    minimum(coherentInWeek);
    minimum(collisions);
    minimum(earlyHours);
    minimum(bonuses);

}

void Scores::setToMaxValuesFrom(const Scores & s) {

    #define maximum(val) val = std::max(val, s.val);

    maximum(coherentInDay);
    maximum(coherentInWeek);
    maximum(collisions);
    maximum(earlyHours);
    maximum(bonuses);

}
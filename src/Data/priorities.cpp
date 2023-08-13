#include "priorities.h"

Priorities::Priorities() :
    keepCoherentInDay(true),
    keepCoherentInWeek(true),
    penaliseBeforeHour(0),
    penaliseManyConsecutiveHours(0),
    penaliseAfterHour(0),
    minutesToBeConsecutive(30) { }

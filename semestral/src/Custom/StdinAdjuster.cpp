#include "StdinAdjuster.h"

Priorities StdinAdjuster::operator()(const Semester & Semester) {
    Priorities result;

    result.keepTogether = true;
    result.enabledPenaliseBeforeTime = false;

    return result;
}
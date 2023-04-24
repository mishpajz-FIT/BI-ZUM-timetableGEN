#include "timeinterval.h"

TimeInterval::TimeStamp::TimeStamp(uint8_t h, uint8_t m): hour(h), minute(m) {
    if (h >= 24 || m >= 60) {
        throw std::invalid_argument("TimeStamp: Hour or minute is larger than possible.");
    }
}

bool TimeInterval::TimeStamp::operator<(const TimeStamp & rhs) {
    return std::tie(hour, minute) < std::tie(rhs.hour, rhs.minute);
}

bool TimeInterval::TimeStamp::operator==(const TimeStamp & rhs) {
    return std::tie(hour, minute) == std::tie(rhs.hour, rhs.minute);
}

TimeInterval::TimeInterval(const enum Day & d, const TimeStamp & s, const TimeStamp & e,
    const Parity & p):
    day(d), startTime(s), endTime(e), parity(p) {

    if (s >= e) {
        throw std::invalid_argument("TimeInterval: Start time is after end time..");
    }
}
#include "Data/timeinterval.h"

TimeInterval::TimeStamp::TimeStamp(uint32_t h, uint32_t m) : hour(h), minute(m) {
    if (h >= 24 || m >= 60) {
        throw std::invalid_argument("TimeStamp hour or minute is larger than possible.");
    }
}

bool TimeInterval::TimeStamp::operator<(const TimeStamp & rhs) const {
    return std::tie(hour, minute) < std::tie(rhs.hour, rhs.minute);
}

bool TimeInterval::TimeStamp::operator==(const TimeStamp & rhs) const {
    return std::tie(hour, minute) == std::tie(rhs.hour, rhs.minute);
}

bool TimeInterval::TimeStamp::operator > (const TimeStamp & rhs) const {
    return rhs < *this;
}

bool TimeInterval::TimeStamp::operator >= (const TimeStamp & rhs) const {
    return (*this > rhs) || (*this == rhs);
}

uint32_t TimeInterval::TimeStamp::valueInMinutes() const {
    return (static_cast<uint32_t>(hour) * 60 + static_cast<uint32_t>(minute));
}

bool TimeInterval::operator < (const TimeInterval & rhs) const {
    size_t dayValue = static_cast<size_t>(day);
    size_t dayRhsValue = static_cast<size_t>(rhs.day);

    // Compare by days and then primarily by start times
    return std::tie(dayValue, startTime, endTime) < std::tie(dayRhsValue, rhs.startTime, rhs.endTime);
}

bool TimeInterval::operator > (const TimeInterval & rhs) const {
    return rhs < *this;
}

TimeInterval::TimeInterval(const enum Day & d, const TimeStamp & s, const TimeStamp & e,
    const Parity & p) :
    day(d), startTime(s), endTime(e), parity(p) {

    if (s >= e) {
        throw std::invalid_argument("TimeInterval start time is after end time");
    }
}
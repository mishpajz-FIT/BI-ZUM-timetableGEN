#include "Data/timeinterval.h"

TimeInterval::TimeStamp::TimeStamp(uint32_t h, uint32_t m) : hour(h), minute(m) {
    if (h >= 24 || m >= 60) {
        throw std::invalid_argument("TimeStamp: Hour or minute is larger than possible.");
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

bool TimeInterval::operator < (const TimeInterval & rhs) const {
    size_t dayValue = static_cast<size_t>(day);
    size_t dayRhsValue = static_cast<size_t>(rhs.day);

    if (dayValue == dayRhsValue) {
        if (startTime == rhs.startTime) {
            return endTime < rhs.endTime;
        }
        return startTime < rhs.startTime;
    }

    return dayValue < dayRhsValue;
}

bool TimeInterval::operator > (const TimeInterval & rhs) const {
    return rhs < *this;
}

TimeInterval::TimeInterval(const enum Day & d, const TimeStamp & s, const TimeStamp & e,
    const Parity & p) :
    day(d), startTime(s), endTime(e), parity(p) {

    if (s >= e) {
        throw std::invalid_argument("TimeInterval: Start time is after end time..");
    }
}
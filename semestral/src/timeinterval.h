#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <cstdint>
#include <stdexcept>
#include <tuple>

class TimeInterval {
public:
    enum class Day {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };

    enum class Parity {
        Even,
        Odd,
        Both
    };

    struct TimeStamp {
        uint8_t hour;
        uint8_t minute;

        TimeStamp(uint8_t h, uint8_t m): hour(h), minute(m) {
            if (h >= 24 || m >= 60) {
                throw std::invalid_argument("TimeStamp: Hour or minute is larger than possible.");
            }
        }

        bool operator <=> (const TimeStamp & rhs) const = default;
    };

private:
    enum Day day;
    TimeStamp startTime;
    TimeStamp endTime;
    enum Parity parity;

public:
    TimeInterval(const enum Day & d, const TimeStamp & s, const TimeStamp & e,
        const Parity & p = TimeInterval::Parity::Both):
        day(d), startTime(s), endTime(e), parity(p) {

        if (s >= e) {
            throw std::invalid_argument("TimeInterval: Start time is after end time..");
        }
    }

};

#endif /* TIMEINTERVAL_H */
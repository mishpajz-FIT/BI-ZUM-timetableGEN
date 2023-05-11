#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <cstdint>
#include <stdexcept>
#include <tuple>

class TimeInterval {
public:
    enum class Day : size_t {
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

        TimeStamp(uint32_t h, uint32_t m);

        bool operator < (const TimeStamp & rhs) const;
        bool operator == (const TimeStamp & rhs) const;
        bool operator > (const TimeStamp & rhs) const;
        bool operator >= (const TimeStamp & rhs) const;
    };

    bool operator < (const TimeInterval & rhs) const;
    bool operator > (const TimeInterval & rhs) const;

    enum Day day;
    TimeStamp startTime;
    TimeStamp endTime;
    enum Parity parity;

    TimeInterval(const enum Day & d, const TimeStamp & s, const TimeStamp & e,
        const Parity & p = TimeInterval::Parity::Both);

};

#endif /* TIMEINTERVAL_H */
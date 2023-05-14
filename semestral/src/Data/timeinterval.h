/**
 * @file timeinterval.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Data structures describing timeintervals and related objects
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <cstdint>
#include <stdexcept>
#include <tuple>

/**
 * @brief Represents a time interval
 *
 */
class TimeInterval {
public:
    /**
     * @brief Day in week
     *
     */
    enum class Day : size_t {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };

    /**
     * @brief Parity
     *
     */
    enum class Parity {
        Even,
        Odd,
        Both
    };

    /**
     * @brief Represents a timestamp
     *
     */
    struct TimeStamp {
        uint8_t hour; // Hour (0-23)
        uint8_t minute; // Minute (0-59)

        /**
         * @brief Construct a new Time Stamp object
         *
         * Hour needs to be in range 0-23, minute in range 0-59.
         *
         * @throws std::invalid_argument if hour or minute is out of range
         *
         * @param h hour
         * @param m minute
         */
        TimeStamp(uint32_t h, uint32_t m);

        bool operator < (const TimeStamp & rhs) const;
        bool operator == (const TimeStamp & rhs) const;
        bool operator > (const TimeStamp & rhs) const;
        bool operator >= (const TimeStamp & rhs) const;

        /**
         * @brief Minutes since midnight
         *
         * @return uint32_t minutes
         */
        uint32_t valueInMinutes() const;
    };

    enum Day day;
    TimeStamp startTime;
    TimeStamp endTime;
    enum Parity parity;

    /**
     * @brief Construct a new Time Interval object
     *
     * @throws std::invalid_argument start time is equal or after end time
     *
     * @param d day
     * @param s start time
     * @param e end time
     * @param p parity
     */
    TimeInterval(const enum Day & d, const TimeStamp & s, const TimeStamp & e,
        const Parity & p = TimeInterval::Parity::Both);

    bool operator < (const TimeInterval & rhs) const;
    bool operator > (const TimeInterval & rhs) const;

};

#endif /* TIMEINTERVAL_H */
/**
 * @file subjects.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Data structures describing all possible timetable entries
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SUBJECTS_H
#define SUBJECTS_H

#include "Data/timeinterval.h"

#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <memory>
#include <set>

struct Schedule;
struct Course;

/**
 * @brief A single entry in a timetable
 *
 * Belongs to some schedule.
 *
 * For example: a parallel of lectures (where lecture is a schedule).
 *
 */
struct Entry {
private:
    double bonus; //!< Bonus or mallus indicating if this entry is desired in the timetable

public:
    uint32_t id; //!< Id of entry, (doesnt have to be unique)
    std::string additionalInformation; //!< All additional infromation for entry
    std::vector<TimeInterval> timeslots; //!< Timeslots of this entry (entry can have multiple timeslots)

    std::weak_ptr<Schedule> schedule; //!< Schedule this entry belongs to
    size_t indexInSchedule; //!< Index this entry has in the schedule it belongs to

    /**
     * @brief Get bonus value of this entry
     *
     * @return double bonus
     */
    double getBonus() const;

    /**
     * @brief Set bonus volue of this entry
     *
     * The value will be capped between -10 and +10
     *
     * @param value bonus
     */
    void setBonus(double value);

    /**
     * @brief Construct a new Entry object
     *
     * @param ix Index this entry has in the schedule it belongs to
     * @param parent Schedule this entry belongs to
     * @param id Id of entry, (doesnt have to be unique)
     */
    Entry(size_t ix, const std::weak_ptr<Schedule> & parent, uint32_t id = 0);
};

/**
 * @brief Represents schedule for a course in semester
 *
 * Groups all entries of same type for a given course
 * For example: lecture, exercise, laboratory.
 *
 */
struct Schedule {
    std::string name; // Name of this schedule, should be identical to the name 
    // under which this schedule is stored in course
    std::vector<std::shared_ptr<Entry>> entriesPtrs; // All grouped entries

    std::weak_ptr<Course> course; // Course this schedule belongs to

    bool ignored; // Should all entries in this schedule be ignored during generation

    /**
     * @brief Construct a new Schedule object
     *
     * @param parent Course this schedule belongs to
     */
    Schedule(const std::weak_ptr<Course> & parent);

    /**
     * @brief Construct a new Schedule object
     *
     * Name should be identical to the name uder which this schedule is stored
     * in course
     *
     * @param parent Course this schedule belongs to
     * @param name Name of this schedule
     */
    Schedule(const std::weak_ptr<Course> & parent, const std::string & name);
};

/**
 * @brief Represents course in semester
 *
 */
struct Course {
    std::string name; // Name of the course
    std::map <std::string, std::shared_ptr<Schedule>> schedulesPtrs; // Schedules of this course
};

/**
 * @brief Represents semester
 *
 */
struct Semester {
    std::vector <std::shared_ptr<Course>> coursesPtrs; // Courses of semester
};

#endif /* SUBJECTS_H */
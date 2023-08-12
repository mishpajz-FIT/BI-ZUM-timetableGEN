/**
 * @file StdinAdjuster.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Adjuster for adjusting properties using a command line
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef STDINADJUSTER_H
#define STDINADJUSTER_H

#include "Utility/adjuster.h"
#include "Data/subjects.h"
#include "Extensions/string_extensions.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>

// Length of visual separator
#define STDIN_ADJUSTER_SEPARATOR_LENGTH 80

/**
 * @brief Adjuster of properties from command line
 *
 */
class StdinAdjuster : public Adjuster {
private:
    enum class ReturnStatus { // Return status for internal functions
        GOOD,
        BAD,
        QUIT
    };

    /**
     * @brief Checks the stdin for failure
     *
     * If a failure is detected, stream is cleared of remaining character and its state.
     * Console is cleared and a message is outputted signaling wrong input.
     *
     * @return true input failed
     * @return false input correct
     */
    bool checkInputForFail();

    /**
     * @brief Adjust specified bool from standard input
     *
     * @param infoText information about the adjusted variable
     * @param value adjusted variable
     */
    void adjustBool(const std::string & infoText, bool & value);

    /**
     * @brief Adjust hour value from standard input
     *
     * Only caps values between 0-23.
     *
     * @param infoText informations about the adjusted variable
     * @param value adjusted hour
     */
    void adjustHour(const std::string & infoText, uint8_t & value);

    /**
     * @brief Adjust variable from standard input
     *
     * Variable ideally should be numeric
     *
     * @tparam T variable type
     * @param infoText informations about the adjusted variable
     * @param value adjusted variable
     */
    template <typename T>
    void adjustValue(const std::string & infoText, T & value);

    /**
     * @brief Make choice of element in vector from standard input
     *
     * @tparam T type of element in vector
     * @param choices vector of choices
     * @param badValue bad value to be returned if not successful
     * @return std::pair<T, StdinAdjuster::ReturnStatus>
     * retrieved value, status (GOOD on success, BAD on failure, QUIT if user wishes to quit)
     */
    template <typename T>
    std::pair<T, StdinAdjuster::ReturnStatus> choiceRetriever(std::vector<T> choices, const T & badValue);

    /**
     * @brief Select and retrieve course from semester
     *
     * @param semester semester to choose course from
     * @return std::pair<std::string StdinAdjuster::ReturnStatus>
     * name of course, status (GOOD on success, BAD on failure, QUIT if user wishes to quit)
     */
    std::pair<std::string, StdinAdjuster::ReturnStatus> retrieveCourse(Semester & semester);

    /**
     * @brief Select and retrieve schedule from course
     *
     * @param course course to choose schedule from
     * @return std::pair<std::shared_ptr<Schedule>, StdinAdjuster::ReturnStatus>
     * pointer to schedule (or nullptr), status (GOOD on success, BAD on failure, QUIT if user wishes to quit)
     */
    std::pair<std::shared_ptr<Schedule>, StdinAdjuster::ReturnStatus> retrieveSchedule(
        Semester & semester,
        const std::string & course);

    /**
     * @brief Select and retrieve entry from schedule
     *
     * @param schedule schedule to choose entry from
     * @return std::pair<std::shared_ptr<Entry>, StdinAdjuster::ReturnStatus>
     * pointer to entry (or nullptr), status (GOOD on success, BAD on failure, QUIT if user wishes to quit)
     */
    std::pair<std::shared_ptr<Entry>, StdinAdjuster::ReturnStatus> retrieveEntry(std::shared_ptr<Schedule> & schedule);

    /**
     * @brief Adjust bonuses values in entries in semester
     *
     * Allows to go through all entries and adjust their bonus (or malus) values
     *
     * @param semester semester to adjust
     */
    void adjustBonuses(Semester & semester);

    /**
     * @brief Adjust ignored value in schedules in semester
     *
     * Allows to go through all schedules and adjust their ignored value
     *
     * @param semester semester to adjust
     */
    void adjustIgnored(Semester & semester);

public:
    Priorities operator()(Semester & semester) override;
};

#endif /* STDINADJUSTER_H */
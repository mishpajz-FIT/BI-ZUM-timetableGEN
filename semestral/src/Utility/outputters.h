/**
 * @file outputters.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Outputter of timetable generation result
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef OUTPUTTER_H
#define OUTPUTTER_H

#include "Data/subjects.h"

#include <vector>
#include <vector>

/**
 * @brief Course and Schedule name
 *
 * @see Course
 * @see Schedule
 *
 * In a pair, first is Course name, second is Schedule name.
 *
 */
using EntryAddress = std::pair<std::string, std::string>;

/**
 * @brief Entry selected for Course and Schedule
 *
 * @see EntryAddress
 *
 * @see Course
 * @see Schedule
 * @see Entry
 *
 * In a pair, first is Course and Schedule name, second is selected Entry.
 *
 */
using EvolutionResult = std::pair<EntryAddress, std::shared_ptr<Entry>>;

/**
 * @brief Outputter of timetable generation result
 *
 * Used for outputting a generation result. Should be subclassed for specific target.
 *
 */
class Outputter {
public:
    Outputter() = default;

    virtual ~Outputter() = default;

    /**
     * @brief Output result
     *
     * @param result result to output
     */
    virtual void output(const std::vector<EvolutionResult> & result) = 0;

};

#endif /* OUTPUTTER_H */
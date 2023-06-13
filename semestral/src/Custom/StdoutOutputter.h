/**
 * @file StdoutOutputter.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Outputter for outputing generated timetable to standard output
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef STDOUTOUTPUTTER_H
#define STDOUTOUTPUTTER_H

#include "Utility/outputters.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <algorithm>
#include <iostream>

/**
 * @brief Outputter for outputing generated timetable to standard output
 *
 * Labels are in English, can be subclassed for other languages.
 *
 */
class StdoutOutputter : Outputter {

protected:
    std::array<std::string, 7> namesOfDays; // Day names mapping

    // Parity mapping
    std::string parityName;
    std::string parityOdd;
    std::string parityEven;

public:

    StdoutOutputter();

    void output(const std::vector<EvolutionResult> & result) override;

};

/**
 * @brief Outputter for outputing generated timetable to standard output with Czech labels
 *
 */
class CS_StdoutOutputter : public StdoutOutputter {

public:
    CS_StdoutOutputter();
};

#endif /* STDOUTOUTPUTTER_H */

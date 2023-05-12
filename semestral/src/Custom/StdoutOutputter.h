#ifndef STDOUTOUTPUTTER_H
#define STDOUTOUTPUTTER_H

#include "Utility/outputters.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <array>
#include <algorithm>
#include <iostream>

class StdoutOutputter: Outputter {

protected:
    std::array<std::string, 7> namesOfDays;

    std::string parityName;
    std::string parityOdd;
    std::string parityEven;

public:

    StdoutOutputter();

    void output(const std::vector<EvolutionResult> & result) override;

};

class CS_StdoutOutputter: public StdoutOutputter {

public:
    CS_StdoutOutputter();
};

#endif /* STDOUTOUTPUTTER_H */

#ifndef OUTPUTER_H
#define OUTPUTER_H

#include "Data/subjects.h"

#include <vector>
#include <vector>

using EntryAddress = std::pair<std::string, std::string>; // course and schedule name
using EvolutionResult = std::pair<EntryAddress, std::shared_ptr<Entry>>;

class Outputter {
public:
    Outputter() = default;

    virtual ~Outputter() = default;

    virtual void output(const std::vector<EvolutionResult> & result) = 0;

};

#endif /* IMPORTERS_H */
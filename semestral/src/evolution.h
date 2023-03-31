#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "subjects.h"
#include "crossover.h"

#include <vector>
#include <tuple>

using Genome = std::vector<uint32_t>;

class Evolution {

    const std::vector<Subject> subjects;

    std::vector<Genome> currentGeneration;

public:

    Evolution(const std::vector<Subject> & s);

    ~Evolution();

    std::vector<std::vector<Subject>> evolve();

private:

    bool mutate(Genome & timetable);

    std::tuple<Genome, Genome> crossover();

};

#endif /* EVOLUTION_H */
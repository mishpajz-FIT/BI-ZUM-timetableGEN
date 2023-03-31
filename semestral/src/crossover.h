#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <vector>

using Genome = std::vector<uint32_t>;

struct Crossover {

    virtual ~Crossover();

    virtual Timetable & perform(Genome & table) = 0;
};

struct SimpleCrossover: Crossover {

    virtual Timetable & perform(Genome & table);
}

#endif /* CROSSOVER_H */
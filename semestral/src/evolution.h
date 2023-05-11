#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "Data/subjects.h"
#include "Evolution/crossover.h"
#include "Evolution/priorities.h"

#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <memory>
#include <algorithm>

using Genome = std::vector<uint32_t>;

using EntryAddress = std::pair<std::string, std::string>; // course and schedule name

class Evolution {

    Semester semester;
    Priorities priorities;

    size_t genomeSize;
    std::vector<std::shared_ptr<Schedule>> genomeIndexToSchedule;
    std::map<EntryAddress, size_t> courseAndScheduleToGenomeIndex;

    std::vector<std::unique_ptr<Crossover>> crossovers;

    std::vector<Genome> currentGeneration;

public:

    Evolution(const Semester & s, const Priorities & p);

    ~Evolution() = default;

    std::vector<std::map<Course, std::map<std::string, uint32_t>>> evolve(size_t generationSize = 100, size_t maxGenerations = 100);

private:

    double fitness(const Genome & genome);

    void createInitialGenerations(size_t generationSize);

    static size_t randomNumber(size_t maxValue);

};

#endif /* EVOLUTION_H */
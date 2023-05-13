#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "Data/subjects.h"
#include "Evolution/crossovers.h"
#include "Evolution/priorities.h"

#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>

using EntryAddress = std::pair<std::string, std::string>; // course and schedule name

using EvolutionResult = std::pair<EntryAddress, std::shared_ptr<Entry>>;

class Evolution {

    Semester semester;
    Priorities priorities;

    size_t genomeSize;
    std::vector<std::shared_ptr<Schedule>> genomeIndexToSchedule;
    std::map<EntryAddress, size_t> courseAndScheduleToGenomeIndex;

    std::vector<std::unique_ptr<Crossover>> crossovers;

public:

    Evolution() = delete;

    Evolution(const Semester & s, const Priorities & p);

    ~Evolution() = default;

    std::vector<EvolutionResult> evolve(size_t generationSize = 100, size_t maxGenerations = 100, bool verbal = true);

private:

    void selection(std::vector<Genome> & newGeneration, size_t generationSize);

    double fitness(const Scores & genomeScore, const Scores & minValues, const Scores & maxValues);

    Scores score(const Genome & genome);

    bool mutate(Genome & genome);

    std::vector<Genome> createInitialGenerations(size_t generationSize) const;

    void loadingBar(std::ostream & stream, size_t value, size_t maxValue) const;

    static size_t randomNumber(size_t maxValue);

};

#endif /* EVOLUTION_H */
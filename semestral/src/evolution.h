#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "Data/subjects.h"
#include "Evolution/crossover.h"

#include <vector>
#include <tuple>
#include <unordered_map>
#include <string>
#include <memory>

using Genome = std::vector<uint32_t>;

class Evolution {

    const std::vector<Course> courses;

    Genome maxValueGenome;
    std::vector<size_t> genomeIndexToCourse;
    std::vector<size_t> genomeIndexToSchedule;

    std::vector<std::unique_ptr<Crossover>> crossovers;

    std::vector<Genome> currentGeneration;

public:

    Evolution(const std::vector<Course> & c);

    ~Evolution() = default;

    std::vector<std::map<Course, std::map<std::string, uint32_t>>> evolve(size_t generationSize = 100, size_t maxGenerations = 100);

private:

    void createInitialGenerations(size_t generationSize);

    static size_t randomNumber(size_t maxValue);

};

#endif /* EVOLUTION_H */
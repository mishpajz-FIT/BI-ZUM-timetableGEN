#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "subjects.h"
#include "crossover.h"

#include <vector>
#include <tuple>
#include <map>
#include <unordered_map>
#include <string>

using Genome = std::vector<uint32_t>;

class Evolution {

    const std::vector<Course> courses;

    Genome maxValueGenome;
    std::vector<size_t> genomeIndexToCourse;
    std::vector<size_t> genomeIndexToClass;

    std::vector<Genome> currentGeneration;
    size_t generationNumber;

public:

    Evolution(const std::vector<Course> & s);

    ~Evolution() = default;

    std::vector<std::map<Course, std::map<std::string, uint32_t>>> evolve(size_t generationSize = 100, size_t maxGenerations = 100);

private:

    void createInitialGenerations(size_t generationSize);

    void applyCrossovers(std::vector<Genome> & generation);

    void applyMutations(std::vector<Genome> & generation);

    static size_t randomNumber(size_t maxValue) ;

};

#endif /* EVOLUTION_H */
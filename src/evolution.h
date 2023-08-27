/**
 * @file evolution.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Evolution algorithm for timetable generation
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef EVOLUTION_H
#define EVOLUTION_H

#include "Data/subjects.h"
#include "Data/priorities.h"
#include "Evolution/crossovers.h"
#include "Evolution/scores.h"

#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <iostream>
#include <functional>

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
 * @brief Evolution algorithm for timetable generation
 *
 */
class Evolution {

    Semester semester; // Semester to generate timetable for
    Priorities priorities; // Specified priorities for generation

    size_t genomeSize; // Calculated genome size
    // Index in genome matches index in this vector, which links to Schedule,
    // the value at this index in Genome is index to entry in this Schedule
    std::vector<std::shared_ptr<Schedule>> genomeIndexToSchedule;
    // Course and Schedule name to index in genome
    std::map<EntryAddress, size_t> courseAndScheduleToGenomeIndex;


    std::vector<std::unique_ptr<Crossover>> crossovers; // Crossover operators

    std::function<void(size_t, size_t)> processing; // Function to be called after every stage of evolution
    // first parameter is current progress value, second is max value

public:

    Evolution() = delete;

    /**
     * @brief Construct a new Evolution object
     *
     * @param s semester for which a timetable will be generated
     * @param p priorities for timetable generation
     * @param proc function to be called after every stage of evolution,
     * where first parameter is current progress value, second is max value
     */
    Evolution(
        const Semester & s,
        const Priorities & p,
        std::function<void(size_t, size_t)> proc = nullptr);

    ~Evolution() = default;

    /**
     * @brief Generate timetable using genetic algorithm
     *
     * @throws std::invalid_argument generation size or number of generations is zero
     *
     * @param generationSize size of generations
     * @param maxGenerations number of generations
     * @return std::vector<EvolutionResult> generated timetable (vector of selected Entries for each Course and its Schedule)
     */
    std::vector<EvolutionResult> evolve(size_t generationSize = 100, size_t maxGenerations = 100);

    /**
     * @brief Get size of genome
     *
     * @return size_t size of genome
     */
    size_t getGenomeSize() const;

private:

    /**
     * @brief Performs selection of best genomes, based on fitness
     *
     * Only amount of genomes up to generation size will be selected.
     *
     * The returned genomes will be sorted by their fitness.
     *
     * @param[inout] newGeneration generation
     * @param generationSize desired size of generation
     */
    void selection(std::vector<Genome> & newGeneration, size_t generationSize) const;

    /**
     * @brief Score given genome
     *
     * @param genome genome to be scored
     * @return Scores score of genome
     */
    Scores score(const Genome & genome) const;

    /**
     * @brief Mutate given genome
     *
     * Genomes are mutated on random place with a certain mutation chance.
     *
     * @param genome genome to be mutated
     * @return true genome was mutated
     * @return false genmoe was mutated
     */
    bool mutate(Genome & genome) const;

    /**
     * @brief Create initial generation
     *
     * Genomes are created randomly.
     *
     * @param generationSize Size of the generation
     * @return std::vector<Genome> initial generation
     */
    std::vector<Genome> createInitialGenerations(size_t generationSize) const;

    /**
     * @brief Random number
     *
     * In range [0, maxValue).
     *
     * @param maxValue upper limit
     * @return size_t random number
     */
    static size_t randomNumber(size_t maxValue);

};

#endif /* EVOLUTION_H */
/**
 * @file crossovers.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Crossovers for genetic algorithm
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CROSSOVERS_H
#define CROSSOVERS_H

#include <vector>
#include <utility>
#include <cstdint>
#include <random>
#include <exception>
#include <set>


#define RANDOM_CROSSOVER_NUMBER_TYPE uint32_t //!< Type for random number
#define RANDOM_CROSSOVER_NUMBER_SIZE 32 //!< Size of type for random number

/**
 * @brief Genome
 *
 */
using Genome = std::vector<uint32_t>;

/**
 * @brief Exception thrown by crossover
 *
 */
struct CrossoverException : public std::exception {

    CrossoverException(std::string message);

    const char * what() const noexcept override;

private:

    std::string msg;
};

/**
 * @brief Crossover operation
 *
 * Abstract class, should be subclassed with specific implementation.
 *
 */
struct Crossover {

    virtual ~Crossover() = default;

    /**
     * @brief Perform crossover of two parents making one child
     *
     * @param lParent parent genome
     * @param rParent parent genome
     * @return Genome child genome
     */
    virtual Genome perform(const Genome & lParent, const Genome & rParent) const = 0;

protected:

    /**
     * @brief Check if genomes of parents are correct
     *
     * @throws CrossoverException if parents got different genome lengths
     *
     * @param lParent parent genome
     * @param rParent parent genome
     * @return size_t genome length
     */
    size_t genomeCheck(const Genome & lParent, const Genome & rParent) const;

    /**
     * @brief Random number
     *
     * @return RANDOM_CROSSOVER_NUMBER_TYPE random number
     */
    static RANDOM_CROSSOVER_NUMBER_TYPE randomNumber();
};

/**
 * @brief Uniform crossover operation
 *
 * Each gene of resulting genome is chosen from eighter parent with equal probabilty.
 *
 */
struct UniformCrossover : Crossover {

    Genome perform(const Genome & lParent, const Genome & rParent) const override;
};

/**
 * @brief Point crossover operation
 *
 * K-point crossover. Crossover points are picked randomly in genome.
 * Genes to one side of crossover point (to end of genome or another crossover point)
 * are taken from one parent, genes on other side are taken from other parent.
 *
 */
struct PointCrossover : Crossover {
private:
    size_t points; // Number of crossover points

public:
    /**
     * @brief Construct a new Point Crossover operator
     *
     * @throws CrossoverException if zero crossover points
     *
     * @param k number of crossover points
     */
    PointCrossover(size_t k);

    /**
     * @brief Perform crossover of two parents making one child
     *
     * @throws CrossoverException if more crossover points than length of genome
     *
     * @param lParent parent genome
     * @param rParent parent genome
     * @return Genome child genome
     */
    Genome perform(const Genome & lParent, const Genome & rParent) const override;
};

#endif /* CROSSOVERS_H */
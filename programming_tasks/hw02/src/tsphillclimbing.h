/**
 * @file tsphillclimbing.h
 * @author Michal Dobes
 * @date 2023-03-02
 *
 * @brief Travelling salesman problem solver using steepest ascent hill climbing with restarts
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef TSPHILLCLIMBING_H
#define TSPHILLCLIMBING_H

#include "distances.h"

#include <vector>
#include <random>
#include <exception>
#include <iostream>
#include <thread>

/**
 * @brief Configuration of travelling salesman visits
 *
 */
struct Configuration {

    std::vector<size_t> indexes; // Vector with indexes that represent indexes of cities visited

    /**
     * @brief Create random configuration for cities
     *
     * Creates a random permutation of indexes
     *
     * @param size Amount of cities
     */
    Configuration(size_t size);

    /**
     * @brief Get all neigbours of configuration
     *
     * Configuration is considered as neighbouring if all indexes are same except for two
     *
     * @return std::vector<Configuration> Neighbours
     */
    std::vector<Configuration> neighbours() const;

    friend std::ostream & operator << (std::ostream & os, const Configuration & configuration); // Print configuration 
                                                                                                // to stream as indexes

private:

    /**
     * @brief Create random permutation of indexes using Fisher-Yates algorithm
     *
     */
    void shuffleFisherYates();

    /**
     * @brief Generate random index for indexes
     *
     * @param largerThan Random index will be equal or larger
     * @param smallerThan Random index will be equal or smaller, special value 0 - smaller limit will be set as indexes size
     * @return size_t Random index
     *
     * @throws out_of_range largerThan > smallerThan
     */
    size_t randomIndex(size_t largerThan = 0, size_t smallerThan = 0) const;
};

/**
 * @brief Calculate total distance needed for visiting all cities exactly one and then to return to starting position
 *
 * @param forConfiguration Configuration (cities which will be visited after each other)
 * @param distances Distances between cities
 * @return unsigned long Distance needed to go around all cities
 */
unsigned long totalDistance(const Configuration & forConfiguration, const Distances & distances);

/**
 * @brief Get best configuration out of vector of configuration based on their total distances
 *
 * @param configurations Vector of configurations
 * @param distances Distances between cities
 * @param verbose Print out all configurations and their distance
 * @return std::pair<Configuration, unsigned long> Configuration with smallest total distance and its total distance
 */
std::pair<Configuration, unsigned long> bestConfiguration(std::vector<Configuration> configurations, const Distances & distances, bool verbose = false);

/**
 * @brief Print visual separator to stdout
 *
 * @param big Bigger separator (another style)
 */
void printSeparator(bool big = false);

/**
 * @brief Hill climbing algorithm minimising total distance needed to visit all cities in distances
 *
 * Steepest climb variant.
 *
 * @param distances Distances between cities
 * @param verbose Print out progress to stdout
 * @return std::pair<Configuration, unsigned long> Configuration with smallest total distance and its total distance
 */
std::pair<Configuration, unsigned long> hillclimbing(const Distances & distances, bool verbose = false);

/**
 * @brief Travelling salesman problem solver using steepest ascent hill climbing with restarts
 *
 * @param distances Distances between cities
 * @param verbose Print out progress to stdout
 * @return unsigned long Smallest distance reached
 */
unsigned long tsp(const Distances & distances, bool verbose = false);

#endif /* TSPHILLCLIMBING_H */
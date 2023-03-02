#ifndef TSPHILLCLIMBING_H
#define TSPHILLCLIMBING_H

#include "distances.h"

#include <deque>
#include <vector>
#include <random>
#include <exception>
#include <iostream>
#include <thread>

struct Configuration {

    std::deque<size_t> indexes;

    Configuration(size_t size);

    void shuffleFisherYates();

    std::vector<Configuration> neighbours() const;

    friend std::ostream & operator << (std::ostream & os, const Configuration & configuration);

private:

    size_t randomIndex(size_t largerThan = 0, size_t smallerThan = 0) const;
};

unsigned long totalDistance(const Configuration & forConfiguration, const Distances & distances);

std::pair<Configuration, unsigned long> bestConfiguration(std::vector<Configuration> configurations, const Distances & distances, bool verbose = false);

void printSeparator(bool big = false);

std::pair<Configuration, unsigned long> hillclimbing(const Distances & distances, bool verbose = false);

void tsp(const Distances & distances, bool verbose = false);

#endif /* TSPHILLCLIMBING_H */
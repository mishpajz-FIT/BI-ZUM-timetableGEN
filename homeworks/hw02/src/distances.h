#ifndef DISTANCES_H
#define DISTANCES_H

#include <vector>
#include <string>

struct Distances {

    std::vector<std::vector<unsigned long>> distances;
    std::vector<std::string> cities;

    Distances(): distances(), cities() { }

    Distances(const std::vector<std::vector<unsigned long>> d, std::vector<std::string> c):
        distances(d),
        cities(c) { }
};

#endif /* DISTANCES_H */
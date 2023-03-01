#ifndef DISTANCES_H
#define DISTANCES_H

#include <vector>
#include <string>
#include <sstream>

struct Distances {

    std::vector<std::vector<unsigned long>> values;
    std::vector<std::string> cities;

    Distances(): values(), cities() { }

    Distances(const std::vector<std::vector<unsigned long>> d, std::vector<std::string> c):
        values(d),
        cities(c) { }
};

#endif /* DISTANCES_H */
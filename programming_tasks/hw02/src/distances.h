#ifndef DISTANCES_H
#define DISTANCES_H

#include <vector>
#include <string>
#include <sstream>

/**
 * @brief Cities and all distances between them
 *
 */
struct Distances {

    std::vector<std::vector<unsigned long>> values; // Distances to other cities 
                                                    // (matrix where [i][j] is distance 
                                                    // from city with index i to city with index j)
    std::vector<std::string> cities;

    Distances(): values(), cities() { }

    Distances(const std::vector<std::vector<unsigned long>> d, std::vector<std::string> c):
        values(d),
        cities(c) { }
};

#endif /* DISTANCES_H */
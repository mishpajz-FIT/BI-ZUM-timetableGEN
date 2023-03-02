/**
 * @brief Traveling salesman problem solver using hill-climbing
 *
 * @author Michal Dobes
 * @date 2023-03-02
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>

#include "utility/distancesloader.h"
#include "tsphillclimbing.h"

int main(int argc, char * argv []) {

    if (argc != 2) { // Check if has path as an argument
        std::cout << "(!) Wrong amount of arguments! Add path to distances as an argument." << std::endl;
        return 1;
    }

    Distances distances;

    try { // Parse distances from file
        distances = readDistances(argv[1]);
    }
    catch (const std::exception & e) {
        std::cout << "(!) " << e.what() << std::endl;
        return 1;
    }

    tsp(distances, true); // Calculate shortest possible distance to cover all cities exactly once and return to back 

    return 0;
}

#include <iostream>

#include "utility/distancesloader.h"
#include "tsphillclimbing.h"

int main(int argc, char * argv []) {

    if (argc != 2) { // Check if has path as an argument
        std::cout << "(!) Wrong amount of arguments! Add path to distances as an argument." << std::endl;
        return 1;
    }

    Distances distances;

    try {
        distances = readDistances(argv[1]);
    }
    catch (const std::exception & e) {
        std::cout << "(!) " << e.what() << std::endl;
        return 1;
    }

    hillclimbing(distances, true);

    return 0;
}

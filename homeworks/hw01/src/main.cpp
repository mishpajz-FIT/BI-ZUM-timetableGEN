#include <iostream>
#include <exception>

#include "mazebuilder.h"

int main(int argc, char * argv []) {

    if (argc != 2) {
        std::cout << "(!) Wrong amount of arguments! Add path to maze as an argument." << std::endl;
        return 1;
    }

    try {
        readMaze(argv[1]);
    }
    catch (const std::exception & e) {
        std::cout << "(!) " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
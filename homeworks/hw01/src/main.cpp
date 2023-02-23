#include <iostream>
#include <exception>

#include "mazebuilder.h"
#include "solver.h"

int main(int argc, char * argv []) {

    if (argc != 2) {
        std::cout << "(!) Wrong amount of arguments! Add path to maze as an argument." << std::endl;
        return 1;
    }

    try {
        DFSSolver dfs(readMaze(argv[1]));
        dfs.solve(true);
    }
    catch (const std::exception & e) {
        std::cout << "(!) " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
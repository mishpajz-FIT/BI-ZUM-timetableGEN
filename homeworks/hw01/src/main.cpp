#include <iostream>
#include <exception>
#include <cstdio>

#include "utility/mazebuilder.h"
#include "solver.h"

class InputException: public std::exception {
protected:
    const std::string message;

public:
    InputException(const std::string & reason): message(reason) { }

    const char * what() const noexcept override {
        return message.c_str();
    }
};

bool selection(const Maze & maze) {

    std::cout << "Please choose algorithm." << std::endl;
    std::cout << " (1) - Random" << std::endl;
    std::cout << " (2) - DFS" << std::endl;
    std::cout << " (3) - BFS" << std::endl;
    std::cout << " (4) - Greedy" << std::endl;
    std::cout << "or (0) to quit." << std::endl;
    std::cout << "Enter algorithm number (to suppress progrees, add 's') e.g. '2 s': ";

    unsigned int number;
    bool silent = false;


    if (!(std::cin >> number)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InputException("Wrong arguments, enter number to choose algorithm.");
    }

    if (std::cin.peek() != '\n') {
        char sign;
        std::cin >> sign;
        if (sign != 's') {
            throw InputException("Wrong sign as an argument (use 's' to suppress proggress).");
        }
        silent = true;
    }

    Solver * currentSolver;
    switch (number) {
        case 0:
            return false;
        case 1:
            currentSolver = new RandomSolver(maze);
            break;
        case 2:
            currentSolver = new DFSSolver(maze);
            break;
        case 3:
            currentSolver = new BFSSolver(maze);
            break;
        case 4:
            currentSolver = new GreedySolver(maze);
            break;
        default:
            throw InputException("Nonexisting selection.");
    }

    currentSolver->solve(!silent);
    delete currentSolver;

    std::cout << "Press q to return to algorithm selection..." << std::endl;
    char sign = (char)(0);
    while (sign != 'q' && sign != 'Q') {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> sign;
    }

    std::cout << "\033[1;1H\033[2J" << std::endl;
    return true;
}

int main(int argc, char * argv []) {

    if (argc != 2) {
        std::cout << "(!) Wrong amount of arguments! Add path to maze as an argument." << std::endl;
        return 1;
    }

    Maze maze;

    try {
        maze = readMaze(argv[1]);
    }
    catch (const std::exception & e) {
        std::cout << "(!) " << e.what() << std::endl;
        return 1;
    }


    bool result = true;
    while (result) {
        try {
            result = selection(maze);
        }
        catch (const std::exception & e) {
            std::cout << "\033[1;1H\033[2J" << std::endl;
            std::cout << "(!) " << e.what() << std::endl;
            result = true;
        }
    }

    return 0;
}
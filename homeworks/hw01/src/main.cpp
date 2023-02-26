/**
 * @brief Maze solver demonstrating different path-finding algorithms.
 *
 * @author Michal Dobes
 * @date 2023-02-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <exception>
#include <cstdio>

#include "utility/mazebuilder.h"
#include "solver.h"

/**
 * @brief Exception when accepting any input from user.
 *
 */
class InputException: public std::exception {
protected:
    const std::string message;

public:
    InputException(const std::string & reason): message(reason) { }

    const char * what() const noexcept override {
        return message.c_str();
    }
};

/**
 * @brief Prompt user for algorithm choice and solve maze using this algorithm.
 *
 * @param maze Maze to run algorithm in
 * @return true Selection should be repeated
 * @return false Program should be quit
 */
bool selection(const Maze & maze) {

    std::cout << "Please choose algorithm." << std::endl;
    std::cout << " (1) - Random" << std::endl;
    std::cout << " (2) - DFS" << std::endl;
    std::cout << " (3) - BFS" << std::endl;
    std::cout << " (4) - Greedy" << std::endl;
    std::cout << " (5) - A*" << std::endl;
    std::cout << "or (0) to quit." << std::endl;
    std::cout << "Enter algorithm number (to suppress progrees, add 's') e.g. '2 s': ";

    unsigned int number;
    bool silent = false;

    // Get algorithm selection from user
    if (!(std::cin >> number)) {
        std::cin.clear(); // Clean stdin for next tries
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw InputException("Wrong arguments, enter number to choose algorithm.");
    }

    // Check if stdin also contains something more
    if (std::cin.peek() != '\n') {
        char sign;
        std::cin >> sign; // Check for silent sign
        if (sign != 's') {
            throw InputException("Wrong sign as an argument (use 's' to suppress proggress).");
        }
        silent = true;
    }

    Solver * currentSolver;
    switch (number) { // Allocate solver based on entered number
        case 0: // Special case siqnalling that the program should be quit
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
        case 5:
            currentSolver = new AStarSolver(maze);
            break;
        default:
            throw InputException("Nonexisting selection.");
    }

    currentSolver->solve(!silent); // Solve and deallocate
    delete currentSolver;

    // Quit from 'result' screen in Solver
    std::cout << "Press q to return to algorithm selection..." << std::endl;
    char sign = (char)(0);
    while (sign != 'q' && sign != 'Q') {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> sign;
    }

    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    return true;
}

int main(int argc, char * argv []) {

    if (argc != 2) { // Check if has path as an argument
        std::cout << "(!) Wrong amount of arguments! Add path to maze as an argument." << std::endl;
        return 1;
    }

    Maze maze;

    try { // Parse maze from file in argument
        maze = readMaze(argv[1]);
    }
    catch (const std::exception & e) {
        std::cout << "(!) " << e.what() << std::endl;
        return 1;
    }


    bool result = true;
    while (result) { // Select and run algorithm on a maze, in a loop until user
                     // decides to quit (signalled by returning false from selection())
        try {
            result = selection(maze);
        }
        catch (const std::exception & e) {
            std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
            std::cout << "(!) " << e.what() << std::endl;
            result = true;
        }
    }

    return 0;
}
/**
 * @file dfs.cpp
 * @author Michal Dobes
 * @date 2023-02-26
 *
 * @brief Depth-first search
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "solver.h"

void DFSSolver::recursiveNodeOpen(MazeCoordinates location, bool verbose) {

    if (solutionFound) { // Check if solution hasnt been found in another function call
        return;
    }

    if (verbose) {
        printProgress();
    }

    for (auto neighbour : maze.neighbours(location)) { // Recursivley call on all neighbours of node
        size_t index = maze.getIndex(neighbour);
        if (nodes[index].state != Solver::Node::State::closed) {
            nodes[index].state = Solver::Node::State::closed;
            nodes[index].previousNodeInPath = location;
            nodesOpened++;

            if (neighbour == maze.end) {
                solutionFound = true;
                return;
            }

            recursiveNodeOpen(neighbour, verbose);
        }
    }
}

void DFSSolver::solve(bool verbose) {
    Solver::solve(verbose);
    solutionFound = false;

    size_t startIndex = maze.getIndex(maze.start);
    nodes[startIndex].state = Solver::Node::State::closed;
    nodesOpened++;

    if (maze.start == maze.end) { // Edge case with start same as end
        printResult();
        return;
    }

    recursiveNodeOpen(maze.start, verbose);
    printResult();
}

void DFSSolver::printProgressInfo() const {
    std::cout << " - o = visited" << std::endl;
}
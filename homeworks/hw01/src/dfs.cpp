#include "solver.h"

void DFSSolver::recProcedure(MazeCoordinates location, bool verbose) {

    if (solutionFound) {
        return;
    }

    if (verbose) {
        printProgress();
    }

    for (auto neighbour : maze.neighbours(location)) {
        size_t index = maze.getIndex(neighbour);
        if (nodes[index].state != Solver::Node::State::closed) {
            nodes[index].state = Solver::Node::State::closed;
            nodes[index].previousNodeInPath = location;
            nodesOpened++;

            if (neighbour == maze.end) {
                solutionFound = true;
                return;
            }

            recProcedure(neighbour, verbose);
        }
    }
}

void DFSSolver::solve(bool verbose) {
    Solver::solve(verbose);
    solutionFound = false;
    recProcedure(maze.start, verbose);
    printResult();
}

void DFSSolver::printProgressInfo() const {
    std::cout << " - o = visited" << std::endl;
}
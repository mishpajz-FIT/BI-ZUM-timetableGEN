#include "solver.h"

void DFSSolver::recProcedure(MazeCoordinates location, bool verbose) {
    nodes[maze.getIndex(location)].state = Solver::Node::State::closed;

    if (verbose) {
        printProgress();
    }

    for (auto neighbour : maze.neighbours(location)) {
        if (nodes[maze.getIndex(neighbour)].state != Solver::Node::State::closed) {
            recProcedure(neighbour, verbose);
        }
    }
}

void DFSSolver::solve(bool verbose) {
    Solver::solve(verbose);
    recProcedure(maze.start, verbose);
}

void DFSSolver::printProgressInfo() const {
    std::cout << " - o = visited" << std::endl;
}
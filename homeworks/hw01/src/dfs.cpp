#include "solver.h"

void DFSSolver::recProcedure(MazeCoordinates location, bool verbose) {
    if (verbose) {
        printProgress();
    }

    for (auto neighbour : maze.neighbours(location)) {
        size_t index = maze.getIndex(neighbour);
        if (nodes[index].state != Solver::Node::State::closed) {
            nodes[index].state = Solver::Node::State::closed;
            nodes[index].previousNodeInPath = location;
            nodesOpened++;
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
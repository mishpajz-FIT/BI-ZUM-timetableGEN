#include "solver.h"

void GreedySolver::recursiveNodeOpen(MazeCoordinates location, bool verbose) {

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

            recursiveNodeOpen(neighbour, verbose);
        }
    }
}
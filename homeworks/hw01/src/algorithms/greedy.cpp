#include "solver.h"

void GreedySolver::recursiveNodeOpen(MazeCoordinates location, bool verbose) {

    if (solutionFound) {
        return;
    }

    if (verbose) {
        printProgress();
    }


    std::vector<Solver::MazeCoordinateDistace> distances;

    for (auto neighbour : maze.neighbours(location)) {
        double dist = maze.end.distanceTo(neighbour);
        distances.emplace_back(neighbour, dist);
    }

    std::sort(distances.begin(), distances.end());

    for (auto neigbourDistance : distances) {
        MazeCoordinates neighbour = neigbourDistance.coordinates;
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
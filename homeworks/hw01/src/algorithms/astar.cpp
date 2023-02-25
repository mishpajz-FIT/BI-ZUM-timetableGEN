#include "solver.h"

void AStarSolver::solve(bool verbose) {
    Solver::solve(verbose);

    auto compare = [ ](const MazeCoordinateDistace & lhs, const MazeCoordinateDistace & rhs) { return rhs < lhs; };
    std::priority_queue<MazeCoordinateDistace, std::vector<MazeCoordinateDistace>, decltype(compare)> queue(compare);

    std::vector<size_t> bestDistances(maze.data.size(), SIZE_T_MAX);

    size_t startIndex = maze.getIndex(maze.start);
    queue.emplace(maze.start, 0);
    bestDistances[startIndex] = 0;
    nodes[startIndex].state = Solver::Node::State::open;

    while (!queue.empty()) {
        MazeCoordinates top = queue.top().coordinates;
        queue.pop();
        size_t topIndex = maze.getIndex(top);
        double topDistance = bestDistances[topIndex];


        if (nodes[topIndex].state == Solver::Node::State::closed) {
            continue;
        }
        nodes[topIndex].state = Solver::Node::State::closed;

        if (top == maze.end) {
            printResult();
            return;
        }

        for (auto neighbour : maze.neighbours(top)) {
            size_t index = maze.getIndex(neighbour);
            if (nodes[index].state != Solver::Node::State::closed) {

                double newDistance = topDistance + 1;
                if (newDistance >= bestDistances[index]) {
                    continue;
                }

                bestDistances[index] = newDistance;

                nodes[index].state = Solver::Node::State::open;
                nodes[index].previousNodeInPath = top;
                nodesOpened++;
                queue.emplace(neighbour, newDistance + potential(neighbour));
            }
        }

        if (verbose) {
            printProgress();
        }
    }

    printResult();
}

double AStarSolver::potential(const MazeCoordinates & from) const {
    return from.manhattanDistanceTo(maze.end);
}
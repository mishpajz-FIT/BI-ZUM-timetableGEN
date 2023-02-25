#include "solver.h"

void GreedySolver::solve(bool verbose) {
    Solver::solve(verbose);

    auto compare = [ ](const MazeCoordinateDistance & lhs, const MazeCoordinateDistance & rhs) { return rhs < lhs; };
    std::priority_queue<MazeCoordinateDistance, std::vector<MazeCoordinateDistance>, decltype(compare)> queue(compare);

    queue.emplace(maze.start, maze.start.distanceTo(maze.end));
    nodes[maze.getIndex(maze.start)].state = Solver::Node::State::open;

    while (!queue.empty()) {
        MazeCoordinates top = queue.top().coordinates;
        queue.pop();
        nodes[maze.getIndex(top)].state = Solver::Node::State::closed;

        if (top == maze.end) {
            printResult();
            return;
        }

        for (auto neighbour : maze.neighbours(top)) {
            size_t index = maze.getIndex(neighbour);
            if (nodes[index].state == Solver::Node::State::undiscovered) {
                nodes[index].state = Solver::Node::State::open;
                nodes[index].previousNodeInPath = top;
                nodesOpened++;
                queue.emplace(neighbour, neighbour.distanceTo(maze.end));
            }
        }

        if (verbose) {
            printProgress();
        }
    }

    printResult();
}
#include "solver.h"

void RandomSolver::solve(bool verbose) {
    Solver::solve(verbose);

    std::vector<MazeCoordinates> open;

    open.emplace_back(maze.start);
    nodes[maze.getIndex(maze.start)].state = Solver::Node::State::open;

    while (!open.empty()) {
        MazeCoordinates top = choose(open);
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
                open.emplace_back(neighbour);
            }
        }

        if (verbose) {
            printProgress();
        }
    }

    printResult();
}

MazeCoordinates RandomSolver::choose(std::vector<MazeCoordinates> & from) const {

    if (from.empty()) {
        throw std::out_of_range("Random Solver tried to choose unavailable open node.");
    }

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<size_t> distribution(0, from.size() - 1);

    size_t randomIndex = distribution(generator);
    MazeCoordinates result = from[randomIndex];
    from.erase(from.begin() + randomIndex);

    return result;
}
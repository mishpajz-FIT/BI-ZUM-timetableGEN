#include "solver.h"

void Solver::printProgress() {
    std::this_thread::sleep_for(std::chrono::milliseconds(std::max(10000 / nodes.size(), (unsigned long)(50))));
    std::cout << "\033[1;1H\033[2J" << std::endl;

    size_t i = 0;
    for (size_t row = 0; row < maze.columnLength; row++) {
        for (size_t column = 0; column < maze.rowLength; column++) {

            if (nodes[i].state == Solver::Node::State::closed) {
                std::cout << 'o';
            } else if (nodes[i].state == Solver::Node::State::open) {
                std::cout << '*';
            } else {
                std::cout << maze.maze[i];
            }

            i++;
        }
        std::cout << std::endl;
    }

    for (size_t p = 0; p < maze.rowLength; p++) {
        std::cout << '-';
    }
    std::cout << std::endl;
}

void DFSSolver::recProcedure(MazeCoordinates v, bool verbose) {
    nodes[maze.getIndex(v)].state = Solver::Node::State::closed;

    if (verbose) {
        printProgress();
    }

    for (auto u : maze.neighbours(v)) {
        if (nodes[maze.getIndex(u)].state != Solver::Node::State::closed) {
            recProcedure(u, verbose);
        }
    }
}

void DFSSolver::solve(bool verbose) {
    recProcedure(maze.start, verbose);
}
#include "solver.h"

void Solver::solve(bool verbose) {
    nodes = std::vector<Solver::Node>(maze.maze.size());
    solveProgress = 0;
}

void Solver::printProgress() {
    std::this_thread::sleep_for(std::chrono::milliseconds(std::max(10000 / nodes.size(), (unsigned long)(50))));
    std::cout << "\033[1;1H\033[2J" << std::endl;

    size_t i = 0;
    for (size_t row = 0; row < maze.columnLength; row++) {
        for (size_t column = 0; column < maze.rowLength; column++) {

            switch (nodes[i].state) {
                case Solver::Node::State::closed:
                    std::cout << 'o';
                    break;
                case Solver::Node::State::open:
                    std::cout << '*';
                    break;
                default:
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
    std::cout << "step number (" << solveProgress++ << ")" << std::endl;

    printProgressInfo();
}

void Solver::printProgressInfo() const {
    std::cout << " - o = closed" << std::endl;
    std::cout << " - * = open" << std::endl;;
}
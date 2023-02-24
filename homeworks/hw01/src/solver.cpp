#include "solver.h"

void Solver::solve(bool verbose) {
    nodes = std::vector<Solver::Node>(maze.maze.size());
    solveProgress = 0;
    nodesOpened = 0;
}

void Solver::printProgress() {
    std::this_thread::sleep_for(std::chrono::milliseconds(std::max(10000 / nodes.size(), (unsigned long)(50))));
    std::cout << "\033[1;1H\033[2J" << std::endl;

    size_t i = 0;
    for (size_t row = 0; row < maze.columnLength; row++) {
        for (size_t column = 0; column < maze.rowLength; column++) {

            if (maze.start.x == column && maze.start.y == row) {
                std::cout << 'S';
            } else if (maze.end.x == column && maze.end.y == row) {
                std::cout << 'E';
            } else {
                printNode(i);
            }

            i++;
        }
        std::cout << std::endl;
    }

    std::cout << "_step_number_(" << solveProgress++ << ")_" << std::endl;
    printProgressInfo();
    printMazeInfo();
    printSeparator();
}

void Solver::printProgressInfo() const {
    std::cout << "  o  = closed" << std::endl;
    std::cout << "  *  = open" << std::endl;;
}

void Solver::printSeparator() const {
    for (size_t p = 0; p < maze.rowLength; p++) {
        std::cout << '-';
    }
    std::cout << std::endl;
}

void Solver::printMazeInfo() const {
    std::cout << "_maze_tiles_" << std::endl;
    std::cout << "  #  = wall" << std::endl;
    std::cout << " ' ' = space" << std::endl;
    std::cout << "  S  = start" << std::endl;
    std::cout << "  E  = end" << std::endl;
}

void Solver::printNode(size_t index) const {
    switch (nodes[index].state) {
        case Solver::Node::State::closed:
            std::cout << 'o';
            break;
        case Solver::Node::State::open:
            std::cout << '*';
            break;
        default:
            if (maze.maze[index] == 'X') {
                std::cout << '#';
            } else {
                std::cout << maze.maze[index];
            }
    }
}
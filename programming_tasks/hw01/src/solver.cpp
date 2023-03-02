#include "solver.h"

bool Solver::MazeCoordinateDistance::operator< (const MazeCoordinateDistance & coordDistance) const {
    return distance < coordDistance.distance;
}

void Solver::solve(bool verbose) {
    nodes = std::vector<Solver::Node>(maze.data.size());
    solveProgress = 0;
    nodesOpened = 0;
}

void Solver::printProgress() {
    // Delay for print to be visible in stdout
    // Use max value based on maze size or minimum that is still possible to display
    std::this_thread::sleep_for(std::chrono::milliseconds(std::max(10000 / nodes.size(), (unsigned long)(50))));

    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console

    size_t i = 0;
    for (size_t row = 0; row < maze.columnLength; row++) {
        for (size_t column = 0; column < maze.rowLength; column++) {

            // Display start or end or character for node in maze
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

    printSeparator();
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
            if (maze.data[index] == 'X') { // Change wall character
                std::cout << '#';
            } else {
                std::cout << maze.data[index];
            }
    }
}

void Solver::printResult() const {

    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console

    Maze mazeCopy(maze); // Copy of maze (to change tiles with path information)

    size_t endIndex = maze.getIndex(maze.end);

    if (nodes[endIndex].state == Solver::Node::State::undiscovered) {
        printSeparator();
        std::cout << "No path has been found." << std::endl;
        std::cout << "_path_lenght_(" << (char)(236) << ")_" << std::endl;
        std::cout << "_nodes_opened_(" << nodesOpened << ")_" << std::endl;
        printSeparator();
        return;
    }

    // Reconstruct and calculate path
    size_t pathLength = 1;
    if (maze.start == maze.end) { // Edge case with start same as end
        pathLength = 0;
    } else {
        MazeCoordinates nextCoords = nodes[endIndex].previousNodeInPath;
        size_t nextCoordsIndex = maze.getIndex(nextCoords);
        while (nextCoords != maze.start) {
            mazeCopy.data[nextCoordsIndex] = 'x'; // Change char in maze's copy in path
            nextCoords = nodes[nextCoordsIndex].previousNodeInPath;
            nextCoordsIndex = maze.getIndex(nextCoords);
            pathLength++;
        }
    }

    // Change char for start and end
    mazeCopy.data[maze.getIndex(maze.start)] = 'S';
    mazeCopy.data[maze.getIndex(maze.end)] = 'E';

    size_t i = 0;
    for (size_t row = 0; row < maze.columnLength; row++) {
        for (size_t column = 0; column < maze.rowLength; column++) {

            if (mazeCopy.data[i] == 'X') {
                std::cout << '#';
            } else {
                std::cout << mazeCopy.data[i];
            }

            i++;
        }
        std::cout << std::endl;
    }

    printSeparator();
    std::cout << "_path_lenght_(" << pathLength << ")_" << std::endl;
    std::cout << "_nodes_opened_(" << nodesOpened << ")_" << std::endl;
    std::cout << "  x  = final path" << std::endl;
    printMazeInfo();
    printSeparator();
}

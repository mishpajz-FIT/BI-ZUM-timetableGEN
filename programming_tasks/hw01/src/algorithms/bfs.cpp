/**
 * @file bfs.cpp
 * @author Michal Dobes
 * @date 2023-02-26
 *
 * @brief Breadth-first search
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "solver.h"

void BFSSolver::solve(bool verbose) {
    Solver::solve(verbose);
    std::queue<MazeCoordinates> queue;

    queue.push(maze.start);
    nodes[maze.getIndex(maze.start)].state = Solver::Node::State::open;
    nodesOpened++;

    while (!queue.empty()) {
        MazeCoordinates top = queue.front();
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
                queue.push(neighbour);
            }
        }

        if (verbose) {
            printProgress();
        }
    }

    printResult();
}
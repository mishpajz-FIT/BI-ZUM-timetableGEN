/**
 * @file astar.cpp
 * @author Michal Dobes
 * @date 2023-02-26
 *
 * @brief A* search with manhattan distance heuristic
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "solver.h"

void AStarSolver::solve(bool verbose) {
    Solver::solve(verbose);

    // Priority queue sorted from smallest distance with potential
    // Allows for duplicit coordinate values (queue can contain same coordinate 
    // twice with two different values when a better distance has been found)
    auto compare = [ ](const MazeCoordinateDistance & lhs, const MazeCoordinateDistance & rhs) { return rhs < lhs; };
    std::priority_queue<MazeCoordinateDistance, std::vector<MazeCoordinateDistance>, decltype(compare)> queue(compare);

    // Vector with smallest distance without potential from start for each position in maze
    std::vector<size_t> bestDistances(maze.data.size(), SIZE_T_MAX);

    // Add start position to queue and best distances
    size_t startIndex = maze.getIndex(maze.start);
    queue.emplace(maze.start, 0);
    bestDistances[startIndex] = 0;
    nodes[startIndex].state = Solver::Node::State::open;
    nodesOpened++;

    while (!queue.empty()) {
        MazeCoordinates top = queue.top().coordinates;
        queue.pop();
        size_t topIndex = maze.getIndex(top);
        double topDistance = bestDistances[topIndex];


        if (nodes[topIndex].state == Solver::Node::State::closed) { // Check if node was already closed before (which
                                                                    // necessarily means that the best possible distance
                                                                    // has been reached), in which case discard this entry
                                                                    // in priority queue
            continue;
        }
        nodes[topIndex].state = Solver::Node::State::closed;

        if (top == maze.end) {
            printResult();
            return;
        }

        for (auto neighbour : maze.neighbours(top)) { // Relax all its neighbours
            size_t index = maze.getIndex(neighbour);
            if (nodes[index].state != Solver::Node::State::closed) {

                // If distance to neighbour is better than previously reached,
                // update best distance reached, path by which was reached and open the neighbour
                double newDistance = topDistance + 1;
                if (newDistance >= bestDistances[index]) {
                    continue;
                }

                bestDistances[index] = newDistance;

                nodes[index].state = Solver::Node::State::open;
                nodes[index].previousNodeInPath = top;
                nodesOpened++;

                // Insert into priority queue with updated distance and calulated potential
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
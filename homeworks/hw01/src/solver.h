#ifndef SOLVER_H
#define SOLVER_H

#include "maze.h"

#include <vector> 
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

struct Solver {

    struct Node {
        enum State {
            undiscovered,
            open,
            closed
        };

        State state;

        Node(): state(Solver::Node::State::undiscovered) { }
    };

    std::vector<Solver::Node> nodes;
    Maze maze;

    Solver(Maze m): maze(m) {
        nodes = std::vector<Solver::Node>(maze.maze.size());
    }

    virtual ~Solver() { }

    virtual void solve(bool verbose = false) = 0;

    void printProgress();
};

struct DFSSolver: public Solver {

    DFSSolver(Maze m): Solver(m) { }

    void recProcedure(MazeCoordinates v, bool verbose);

    void solve(bool verbose = false) override;
};

#endif /* SOLVER */
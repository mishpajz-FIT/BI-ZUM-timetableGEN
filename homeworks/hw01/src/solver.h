#ifndef SOLVER_H
#define SOLVER_H

#include "maze.h"

#include <vector> 
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <queue>

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

    Maze maze;
    std::vector<Solver::Node> nodes;

    unsigned long solveProgress;

    Solver(Maze m): maze(m), nodes(), solveProgress(0) { }

    virtual ~Solver() { }

    virtual void solve(bool verbose = false);

    void printProgress();

    virtual void printProgressInfo() const;
};

struct DFSSolver: public Solver {

    DFSSolver(Maze m): Solver(m) { }

    void recProcedure(MazeCoordinates v, bool verbose);

    void solve(bool verbose = false) override;

    void printProgressInfo() const override;
};

struct BFSSolver: public Solver {

    BFSSolver(Maze m): Solver(m) { }

    void solve(bool verbose = false) override;
};

#endif /* SOLVER */
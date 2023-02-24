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

protected:
    unsigned long solveProgress;
    unsigned long nodesOpened;

public:

    struct Node {
        enum State {
            undiscovered,
            open,
            closed
        };

        State state;
        MazeCoordinates previousNodeInPath;

        Node(): state(Solver::Node::State::undiscovered), previousNodeInPath(-1, -1) { }
    };

    Maze maze;
    std::vector<Solver::Node> nodes;

    Solver(Maze m): solveProgress(0), nodesOpened(0), maze(m), nodes() { }

    virtual ~Solver() { }

    virtual void solve(bool verbose = false);

protected:
    void printProgress();

    virtual void printProgressInfo() const;

private:
    void printSeparator() const;

    void printMazeInfo() const;

};

struct DFSSolver: public Solver {

    DFSSolver(Maze m): Solver(m) { }

    void solve(bool verbose = false) override;

private:
    void recProcedure(MazeCoordinates v, bool verbose);

protected:
    void printProgressInfo() const override;
};

struct BFSSolver: public Solver {

    BFSSolver(Maze m): Solver(m) { }

    void solve(bool verbose = false) override;
};

#endif /* SOLVER */
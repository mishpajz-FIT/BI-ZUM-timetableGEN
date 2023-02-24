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
        MazeCoordinates previousNodeInPath;

        Node(): state(Solver::Node::State::undiscovered), previousNodeInPath(-1, -1) { }
    };

    Maze maze;
    std::vector<Solver::Node> nodes;

protected:
    unsigned long solveProgress;
    unsigned long nodesOpened;

public:

    Solver(const Maze & m): maze(m), nodes(), solveProgress(0), nodesOpened(0) { }

    virtual ~Solver() { }

    virtual void solve(bool verbose = false);

protected:

    void printProgress();

    void printResult() const;

    virtual void printProgressInfo() const;

private:
    void printSeparator() const;

    void printMazeInfo() const;

    void printNode(size_t index) const;
};

struct DFSSolver: public Solver {

    DFSSolver(Maze m): Solver(m), solutionFound(false) { }

    void solve(bool verbose = false) override;

private:

    bool solutionFound;

    void recProcedure(MazeCoordinates v, bool verbose);

protected:
    void printProgressInfo() const override;
};

struct BFSSolver: public Solver {

    BFSSolver(Maze m): Solver(m) { }

    void solve(bool verbose = false) override;
};

#endif /* SOLVER */
#ifndef SOLVER_H
#define SOLVER_H

#include "maze.h"

#include <vector> 
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <random>

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

    struct MazeCoordinateDistace {

        MazeCoordinates coordinates;
        double distance;

        MazeCoordinateDistace(): coordinates(-1, -1), distance(0.0) { }

        MazeCoordinateDistace(const MazeCoordinates & coords, double d = 0.0): coordinates(coords), distance(d) { }

        bool operator < (const MazeCoordinateDistace & coordDistance) const;
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

struct RandomSolver: public Solver {

    RandomSolver(const Maze & m): Solver(m) { }

    void solve(bool verbose = false) override;

private:

    MazeCoordinates choose(std::vector<MazeCoordinates> & from) const;
};

struct DFSSolver: public Solver {

    DFSSolver(const Maze & m): Solver(m), solutionFound(false) { }

    void solve(bool verbose = false) override;

protected:

    bool solutionFound;

    virtual void recursiveNodeOpen(MazeCoordinates v, bool verbose);

    void printProgressInfo() const override;
};

struct BFSSolver: public Solver {

    BFSSolver(const Maze & m): Solver(m) { }

    void solve(bool verbose = false) override;
};

struct GreedySolver: public Solver {

    GreedySolver(const Maze & m): Solver(m) { }

    void solve(bool verbose = false) override;
};
#endif /* SOLVER */
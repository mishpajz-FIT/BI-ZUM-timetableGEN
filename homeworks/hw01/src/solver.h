#ifndef SOLVER_H
#define SOLVER_H

#include "maze.h"

struct Solver {
    virtual void solve(const Maze & maze, bool verbose = false) = 0;
};

struct DFSSolver: public Solver {

    void solve(const Maze & maze, bool verbose = false) override;
};

#endif /* SOLVER */
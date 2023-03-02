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

/**
 * @brief Solver used for finding path in maze.
 *
 * Abstract method used to derive from
 *
 */
struct Solver {

    /**
     * @brief Node in maze.
     *
     * Stores its state during algorithm run and previous node in found path
     *
     */
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

    /**
     * @brief Cordinates in maze with stored distance information.
     *
     * Impements comparison operator based on distance.
     *
     */
    struct MazeCoordinateDistance {

        MazeCoordinates coordinates;
        double distance;

        MazeCoordinateDistance(): coordinates(-1, -1), distance(0.0) { }

        MazeCoordinateDistance(const MazeCoordinates & coords, double d = 0.0): coordinates(coords), distance(d) { }

        bool operator < (const MazeCoordinateDistance & coordDistance) const; // Comparison operator based on distance.
    };

    Maze maze; // Maze to perform algorithm in
    std::vector<Solver::Node> nodes; // Same size as maze.data, represents algorithms run's values for mazes tiles, resets during each run

protected:
    unsigned long solveProgress; // Progress valued used during progress printing (to count steps)
    unsigned long nodesOpened; // Represents number of nodes (maze's tiles) "opened" (processed) during algorithm's run, resets during each run

public:

    Solver(const Maze & m): maze(m), nodes(), solveProgress(0), nodesOpened(0) { }

    virtual ~Solver() { }

    /**
     * @brief Find path in maze from start to end.
     *
     * Resets all necessary variables
     *
     * Prints out result to stdout at the end, with option to print progress aswell
     * When printing progress, performance is severly limited so progress is visible on stdout
     *
     * @note When overloading this function, this function needs to be called on generic Solver (parent) at the beginning
     * of overloaded solve
     *
     * @param verbose Print progress during run
     */
    virtual void solve(bool verbose = false);

protected:

    /**
     * @brief Print maze with its information in nodes.
     *
     * Prints progress of algorithm (open, closed tiles etc.)
     * Uses solveProgress variable for internal information
     *
     */
    void printProgress();

    /**
     * @brief Prints maze with found path from start to finish.
     *
     */
    void printResult() const;

    /**
     * @brief Print information about special characters used in printProgress.
     *
     * Characters used in maze are not printed
     *
     */
    virtual void printProgressInfo() const;

private:

    /**
     * @brief Print separator with same lenght as maze row lenght.
     *
     */
    void printSeparator() const;

    /**
     * @brief Print information about characters used when printing maze.
     *
     */
    void printMazeInfo() const;

    /**
     * @brief Print character associated with tile at given index.
     *
     * @param index Index of node in maze
     */
    void printNode(size_t index) const;
};

//LINK - Random search

struct RandomSolver: public Solver {

    RandomSolver(const Maze & m): Solver(m) { }

    void solve(bool verbose = false) override;

private:

    /**
     * @brief Chooses random coordinate from vector of coordinates
     *
     * Coordinate is also removed from this vector
     *
     * @param from Vector of coordinates
     * @return MazeCoordinates Random coordinate
     *
     * @throws out_of_range Vector is empty
     */
    MazeCoordinates choose(std::vector<MazeCoordinates> & from) const;
};

//LINK - Depth-first search

struct DFSSolver: public Solver {

    DFSSolver(const Maze & m): Solver(m), solutionFound(false) { }

    void solve(bool verbose = false) override;

protected:

    bool solutionFound; // Variable for algorithm to determine to stop after end has been found

    /**
     * @brief Recursive depth-first search
     *
     * @param v Coordinates to continue serching in
     * @param verbose Print progress during run
     */
    virtual void recursiveNodeOpen(MazeCoordinates v, bool verbose);

    void printProgressInfo() const override;
};

//LINK - Breadth-first search

struct BFSSolver: public Solver {

    BFSSolver(const Maze & m): Solver(m) { }

    void solve(bool verbose = false) override;
};

//LINK - Greedy search

struct GreedySolver: public Solver {

    GreedySolver(const Maze & m): Solver(m) { }

    void solve(bool verbose = false) override;
};

//LINK - A* search

struct AStarSolver: public Solver {

    AStarSolver(const Maze & m): Solver(m) { }

    void solve(bool verbose = false) override;

private:

    /**
     * @brief Potential calculation for node in maze
     *
     * Uses manhattan distance to end as potential
     *
     * @param from Coordinate of node
     * @return double Potential
     */
    double potential(const MazeCoordinates & from) const;
};
#endif /* SOLVER */
#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <exception>
#include <list>
#include <cmath>

/**
 * @brief Exception for any operation with Maze.
 *
 */
class MazeException: public std::exception {
protected:
    const std::string message;

public:
    MazeException(const std::string & reason): message(reason) { }

    const char * what() const noexcept override;
};

/**
 * @brief Coordinates in maze.
 *
 * x and y (integer) coordinates
 * only nonnegative values allowed
 *
 */
struct MazeCoordinates {
    unsigned long x;
    unsigned long y;

    MazeCoordinates(unsigned long xCoord, unsigned long yCoord): x(xCoord), y(yCoord) { }

    /**
     * @brief Get index for this coordinate in case of a linear representation.
     *
     * Converts coordinate to index for array, where rows are stored behind each other
     *
     * @param rowLenght Lenght of an row in maze.
     * @return size_t Index
     */
    size_t getIndex(size_t rowLenght) const;

    /**
     * @brief Checks if coordinate is in maze's dimension.
     *
     * @param xSize Maze's x-size
     * @param ySize Maze's y-size
     * @return true Coordinate is valid
     * @return false Coordinate is not valid
     */
    bool validityCheck(size_t xSize, size_t ySize) const;

    auto operator <=> (const MazeCoordinates &) const = default; // Automatically generated standard comparison operators

    /**
     * @brief Euclidean distance from this coordinate to another.
     *
     * @param coords Other coordinates to calculate distance to
     * @return double Distance
     */
    double euclideanDistanceTo(const MazeCoordinates & coords) const;

    /**
     * @brief Manhattan distance from this coordinate to another.
     *
     * Distance along axis
     * Sum of the absolute differences of their coordinates
     *
     * @param coords Other coordinates to calculate distance to
     * @return double Distance
     */
    double manhattanDistanceTo(const MazeCoordinates & coords) const;
};

/**
 * @brief Rectangular maze.
 *
 */
struct Maze {
    std::vector<char> data; // Tiles in maze, (2D squashed into linear representation.)
    MazeCoordinates start;
    MazeCoordinates end;

    size_t rowLength;
    size_t columnLength;

    Maze(): data(), start(-1, -1), end(-1, -1), rowLength(0), columnLength(0) { }

    /**
     * @brief Add char (tile) into maze.
     *
     * @param c Char (tile) of maze, either ' ' or 'X'
     *
     * @throws MazeException Char is not allowed
     */
    void pushChar(char c);

    /**
     * @brief Check if maze is valid.
     *
     * Checks if data has correct size in regards to row and column length
     * Checks if start and end tiles are at valid position
     *
     * @return true Maze is valid
     * @return false Start or end is outside maze's bounds
     *
     * @throws MazeException Maze dimensions are different with regards to tiles supplied
     * @throws MazeException Start or end is not in free space in maze
     */
    bool validityCheck() const;

    /**
     * @brief Get index for coordinate in case of a linear representation.
     *
     * Converts coordinate to index for array, where rows are stored behind each other
     *
     * @param location Coordinate to convert
     * @return size_t Index
     */
    size_t getIndex(const MazeCoordinates & location) const;

    /**
     * @brief Get list of all valid neighbours for given coordinate
     *
     * Returns all neighbours in closest vicinity (top, left, right, bottom), that are free and in maze's bounds
     * Neighbours are returned in anticlockwise direction
     *
     * @param location Coordinate which neighbours should be retrieved
     * @return std::list<MazeCoordinates> List with coordinates as neighbours
     */
    std::list<MazeCoordinates> neighbours(const MazeCoordinates & location) const;
};

#endif /* MAZE_H */
#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <exception>
#include <list>
#include <cmath>
#

class MazeException: public std::exception {
protected:
    const std::string message;

public:
    MazeException(const std::string & reason): message(reason) { }

    const char * what() const noexcept override;
};


struct MazeCoordinates {
    unsigned long x;
    unsigned long y;

    MazeCoordinates(unsigned long xCoord, unsigned long yCoord): x(xCoord), y(yCoord) { }

    size_t getIndex(size_t rowLenght) const;

    bool validityCheck(size_t xSize, size_t ySize) const;

    auto operator <=> (const MazeCoordinates &) const = default;

    double distanceTo(const MazeCoordinates & coords) const;

    double manhattanDistanceTo(const MazeCoordinates & coords) const;
};

struct Maze {
    std::vector<char> data;
    MazeCoordinates start;
    MazeCoordinates end;

    size_t rowLength;
    size_t columnLength;

    Maze(): data(), start(-1, -1), end(-1, -1), rowLength(0), columnLength(0) { }

    void pushChar(char c);

    bool validityCheck() const;

    size_t getIndex(const MazeCoordinates & location) const;

    std::list<MazeCoordinates> neighbours(const MazeCoordinates & location) const;
};

#endif /* MAZE_H */
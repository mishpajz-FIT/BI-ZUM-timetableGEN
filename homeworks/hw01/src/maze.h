#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <exception>

class MazeException: public std::exception {
protected:
    const std::string message;

public:
    MazeException(const std::string & reason): message(reason) { }

    const char * what() const noexcept override;
};


class MazeCoordinates {
    unsigned long x;
    unsigned long y;

public:
    MazeCoordinates(unsigned long xCoord, unsigned long yCoord): x(xCoord), y(yCoord) { }

    size_t getIndex(size_t rowLenght) const;

    bool validityCheck(size_t xSize, size_t ySize) const;
};

struct Maze {
    std::vector<char> maze;
    MazeCoordinates start;
    MazeCoordinates end;

    size_t rowLength;
    size_t columnLength;

    Maze(): maze(), start(-1, -1), end(-1, -1), rowLength(0), columnLength(0) { }

    void pushChar(char c);

    bool validityCheck() const;
};

#endif /* MAZE_H */
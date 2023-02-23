#include "maze.h"

const char * MazeException::what() const noexcept {
    return message.c_str();
}

size_t MazeCoordinates::getIndex(size_t rowLenght) const {
    return y * rowLenght + x;
}

bool MazeCoordinates::validityCheck(size_t xSize, size_t ySize) const {
    return xSize > x && ySize > y;
}

void Maze::pushChar(char c) {
    if (c != 'X' && c != ' ') {
        throw MazeException("Unknown character.");
    }

    maze.push_back(c);
}

bool Maze::validityCheck() const {
    if (rowLength * columnLength != maze.size()) {
        throw MazeException("Incorrect maze dimensions.");
    }

    if (!start.validityCheck(rowLength, columnLength) || !end.validityCheck(rowLength, columnLength)) {
        return false;
    }

    return true;
}
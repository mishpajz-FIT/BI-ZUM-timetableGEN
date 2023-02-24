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

double MazeCoordinates::distanceTo(const MazeCoordinates & coords) const {
    return sqrt(pow((coords.x - x), 2.0) + pow((coords.y - y), 2.0));
}

void Maze::pushChar(char c) {
    if (c != 'X' && c != ' ') {
        throw MazeException("Unknown character.");
    }

    data.push_back(c);
}

bool Maze::validityCheck() const {
    if (rowLength * columnLength != data.size()) {
        throw MazeException("Incorrect maze dimensions.");
    }

    if (data[getIndex(start)] != ' ' || data[getIndex(end)] != ' ') {
        throw MazeException("Start or end position at impossible location in maze. (Probably inside wall)");
    }

    if (!start.validityCheck(rowLength, columnLength) || !end.validityCheck(rowLength, columnLength)) {
        return false;
    }

    return true;
}

size_t Maze::getIndex(const MazeCoordinates & location) const {
    return location.getIndex(rowLength);
}

std::list<MazeCoordinates> Maze::neighbours(const MazeCoordinates & location) const {
    std::list<MazeCoordinates> result;

    MazeCoordinates coordinate(location);

    coordinate = location;
    coordinate.y += 1;
    if (coordinate.validityCheck(rowLength, columnLength) && data[getIndex(coordinate)] == ' ') {
        result.push_back(coordinate);
    }

    coordinate = location;
    coordinate.x -= 1;
    if (coordinate.validityCheck(rowLength, columnLength) && data[getIndex(coordinate)] == ' ') {
        result.push_back(coordinate);
    }

    coordinate = location;
    coordinate.y -= 1;
    if (coordinate.validityCheck(rowLength, columnLength) && data[getIndex(coordinate)] == ' ') {
        result.push_back(coordinate);
    }

    coordinate.x += 1;
    if (coordinate.validityCheck(rowLength, columnLength) && data[getIndex(coordinate)] == ' ') {
        result.push_back(coordinate);
    }

    return result;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <cstring>
#include <exception>

class MazeBuilderException: public std::exception {
    const std::string message;

public:
    MazeBuilderException(const std::string & reason): message(reason) { }

    const char * what() const noexcept override {
        return message.c_str();
    }
};

class MazeCoordinates {
    unsigned long x;
    unsigned long y;

public:
    MazeCoordinates(unsigned long xCoord, unsigned long yCoord): x(xCoord), y(yCoord) { }

    size_t getIndex(size_t rowLenght) const {
        return y * rowLenght + x;
    }

    bool validityCheck(size_t xSize, size_t ySize) const {
        return xSize > x && ySize > y;
    }
};

struct Maze {
    std::vector<char> maze;
    MazeCoordinates start;
    MazeCoordinates end;

    size_t rowLength;
    size_t columnLength;

    Maze(): maze(), start(-1, -1), end(-1, -1), rowLength(0), columnLength(0) { }

    void pushChar(char c) {
        if (c != 'X' && c != ' ') {
            throw MazeBuilderException("Unknown character.");
        }

        maze.push_back(c);
    }

    bool validityCheck() const {
        if (rowLength * columnLength != maze.size()) {
            throw MazeBuilderException("Incorrect maze dimensions.");
        }

        if (!start.validityCheck(rowLength, columnLength) || !end.validityCheck(rowLength, columnLength)) {
            return false;
        }

        return true;
    }
};

Maze readMaze(const std::string & filePath) {

    std::fstream file;
    file.open(filePath);

    if (!file.is_open()) {
        throw MazeBuilderException("File could not be opened.");
    }

    std::cout << "Reading from file: " << filePath << std::endl;

    std::string buffer;
    unsigned long x;
    unsigned long y;
    char description[21];
    bool setEnd = false;
    bool setStart = false;

    Maze loadedMaze;

    while (std::getline(file, buffer)) {
        if (sscanf(buffer.c_str(), "%20s %lu, %lu", description, &x, &y) == 3) {
            if (strcmp(description, "start") == 0) {
                if (setStart) {
                    throw MazeBuilderException("More than one location (start) setting in file.");
                }
                setStart = true;
                loadedMaze.start = MazeCoordinates(x, y);
                continue;
            } else if (strcmp(description, "end") == 0) {
                if (setEnd) {
                    throw MazeBuilderException("More than one location (end) setting in file.");
                }
                setEnd = true;
                loadedMaze.end = MazeCoordinates(x, y);
                continue;
            }

            throw MazeBuilderException("Unknown information in file.");
        }

        if (loadedMaze.rowLength == 0) {
            loadedMaze.rowLength = buffer.length();
        } else if (loadedMaze.rowLength != buffer.length()) {
            throw MazeBuilderException("Inconsistent line length in file.");
        }

        for (auto c : buffer) {
            loadedMaze.pushChar(c);
        }
        loadedMaze.columnLength++;
    }

    if (loadedMaze.maze.size() == 0) {
        throw MazeBuilderException("Inconsistent line length in file.");
    }

    if (!loadedMaze.validityCheck()) {
        throw MazeBuilderException("Wrong locations (start or end) in maze.");
    }

    return loadedMaze;
}


int main(int argc, char * argv []) {

    if (argc != 2) {
        std::cout << "(!) Wrong amount of arguments! Add path to maze as an argument." << std::endl;
        return 1;
    }

    try {
        readMaze(argv[1]);
    }
    catch (const std::exception & e) {
        std::cout << "(!) " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
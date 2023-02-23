#include "mazebuilder.h"

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
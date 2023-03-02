#include "utility/mazebuilder.h"

Maze readMaze(const std::string & filePath) {
    std::fstream file;
    file.open(filePath);

    if (!file.is_open()) {
        throw MazeBuilderException("File could not be opened.");
    }

    std::cout << "Reading from file: " << filePath << std::endl;

    // VBuffer variables
    std::string buffer;
    unsigned long x;
    unsigned long y;
    char description[21];

    // Information loaded from file
    bool setEnd = false;
    bool setStart = false;
    Maze loadedMaze;

    while (std::getline(file, buffer)) {
        if (sscanf(buffer.c_str(), "%20s %lu, %lu", description, &x, &y) == 3) { // Try to parse start or end location
            if (strcmp(description, "start") == 0) { // Parse start location
                if (setStart) {
                    throw MazeBuilderException("More than one location (start) setting in file.");
                }
                setStart = true;
                loadedMaze.start = MazeCoordinates(x, y);
                continue;
            } else if (strcmp(description, "end") == 0) { // Parse end location
                if (setEnd) {
                    throw MazeBuilderException("More than one location (end) setting in file.");
                }
                setEnd = true;
                loadedMaze.end = MazeCoordinates(x, y);
                continue;
            }

            throw MazeBuilderException("Unknown information in file.");
        }

        if (loadedMaze.rowLength == 0) { // Load row length from first row
            loadedMaze.rowLength = buffer.length();
        } else if (loadedMaze.rowLength != buffer.length()) { // Check if all rows have same length
            throw MazeBuilderException("Inconsistent line length in file.");
        }

        for (auto c : buffer) { // Store all characters in maze
            loadedMaze.pushChar(c);
        }
        loadedMaze.columnLength++;
    }

    // Validity checks
    if (loadedMaze.data.size() == 0) {
        throw MazeBuilderException("Inconsistent line length in file.");
    }

    if (!loadedMaze.validityCheck()) {
        throw MazeBuilderException("Wrong locations (start or end) in maze.");
    }

    return loadedMaze;
}
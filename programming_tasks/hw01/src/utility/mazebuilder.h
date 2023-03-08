#ifndef MAZEBUILDER_H
#define MAZEBUILDER_H

#include "../maze.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

/**
 * @brief Exception when parsing Maze from file.
 *
 */
class MazeBuilderException: public MazeException {
public:
    MazeBuilderException(const std::string & reason): MazeException(reason) { }
};

/**
 * @brief Parse Maze from file
 *
 * @param filePath Path to file with maze
 * @return Maze Parsed maze
 *
 * @throws MazeBuilderException File unreadable
 * @throws MazeBuilderException Wrong information about start or end position
 * @throws MazeBuilderException Wrong characters used in description of maze
 * @throws MazeBuilderException Wrong formatting of file
 *
 */
Maze readMaze(const std::string & filePath);

#endif /* MAZEBUILDER_H */
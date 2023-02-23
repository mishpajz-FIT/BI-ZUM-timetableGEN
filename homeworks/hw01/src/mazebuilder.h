#ifndef MAZEBUILDER_H
#define MAZEBUILDER_H

#include "maze.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

class MazeBuilderException: public MazeException {
public:
    MazeBuilderException(const std::string & reason): MazeException(reason) { }
};

Maze readMaze(const std::string & filePath);

#endif /* MAZEBUILDER_H */
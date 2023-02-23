#include <iostream>
#include <fstream>
#include <vector>
#include <string>

class MazeBuilderException: public std::exception {
    const std::string message;

public:
    MazeBuilderException(const std::string & reason): message(reason) { }

    const char * what() const noexcept override {
        return message.c_str();
    }
};

std::vector<std::string> readMaze(const std::string & filePath) {

    std::fstream file;
    file.open(filePath);

    if (!file.is_open()) {
        throw MazeBuilderException("File could not be opened!");
    }

    std::cout << "Reading from file: " << filePath << std::endl;



    return { "a" };
}


int main(int argc, char * argv []) {

    if (argc != 2) {
        std::cout << "Wrong amount of arguments! Add path to maze as an argument." << std::endl;
        return 1;
    }

    try {
        readMaze(argv[1]);
    }
    catch (const std::exception & e) {
        std::cout << "(!) " << e.what() << std::endl;
    }

    return 0;
}
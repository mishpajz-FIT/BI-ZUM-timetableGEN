#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> readFile(const std::string & fileName) {
    return { "a" };
}


int main(int argc, char * argv []) {

    if (argc != 2) {
        std::cout << "Wrong amount of arguments! Add path to maze as an argument." << std::endl;
        return 1;
    }

    try {
        readFile(argv[1]);
    }
    catch (...) {
        std::cout << "Error" << std::endl;
    }

    return 0;
}
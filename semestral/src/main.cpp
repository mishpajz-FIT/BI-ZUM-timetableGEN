#include "Custom/FITCTUFileImporter.h"
#include "Custom/StdinAdjuster.h"
#include "Custom/StdoutOutputter.h"
#include "evolution.h"

#include <iostream>
#include <vector>
#include <string>

#define SEPARATOR_LENGTH 80
#define WINDOW_HEIGHT 100

Semester loadSemester(std::string & logo) {
    std::cout << logo << std::endl;
    std::cout << std::string(SEPARATOR_LENGTH, '=') << '\n';
    std::cout << "Enter a path to a file with parallels of courses from KOS FIT CTU (in Czech):\n";
    std::cout << std::string(SEPARATOR_LENGTH, '_') << std::endl;
    std::string filepath;
    std::cin >> filepath;


    Semester result;

    try {
        CS_FITCTUFileImporter importer(filepath);
        result = importer.import();
    }
    catch (const std::exception & e) {
        std::cerr << " (!) Problem loading from file: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    return result;
}

Priorities loadPriorities(std::string & logo, Semester & semester) {
    std::cout << logo << std::endl;
    std::cout << std::string(SEPARATOR_LENGTH, '=') << '\n';
    std::cout << "Adjust priorities for schedule properties? (y/n):\n";
    std::cout << std::string(SEPARATOR_LENGTH, '_') << std::endl;

    Priorities result;
    char choice;
    do {
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            break;
        } else if (choice == 'n' || choice == 'N') {
            return result;
        }
    } while (true);

    StdinAdjuster adjuster;
    result = adjuster(semester);

    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    return result;
}

void evolve(std::string & logo, Semester & semester, Priorities & priorities) {
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    std::cout << logo << std::endl;
    Evolution evolution(semester, priorities);
    std::vector<EvolutionResult> result = evolution.evolve(150, 300);

    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    std::cout << logo << std::endl;
    CS_StdoutOutputter outputter;
    outputter.output(result);
}

int main() {
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console

    std::string logo;
    logo += "   __  _                __        __    __                    \n";
    logo += "  / /_(_)___ ___  ___  / /_____ _/ /_  / /__  ____ ____  ____ \n";
    logo += " / __/ / __ `__ \\/ _ \\/ __/ __ `/ __ \\/ / _ \\/ __ `/ _ \\/ __ \\\n";
    logo += "/ /_/ / / / / / /  __/ /_/ /_/ / /_/ / /  __/ /_/ /  __/ / / /\n";
    logo += "\\__/_/_/ /_/ /_/\\___/\\__/\\__,_/_.___/_/\\___/\\__, /\\___/_/ /_/ \n";
    logo += "                                           /____/             \n";

    Semester semester = loadSemester(logo);
    Priorities priorities = loadPriorities(logo, semester);
    evolve(logo, semester, priorities);

    return 0;
}
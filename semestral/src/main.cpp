/**
 * @file main.cpp
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief FIT CTU Timetable generator using genetic algorithm
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "Custom/FITCTUFileImporter.h"
#include "Custom/StdinAdjuster.h"
#include "Custom/StdoutOutputter.h"
#include "evolution.h"

#include <iostream>
#include <vector>
#include <string>

// Length of visual separator on output
#define SEPARATOR_LENGTH 80

// Multiplier of generation size (multiplies genome size)
#define GENERATION_SIZE_MULTIPLIER 4

// Default count of generations
#define GENERATION_COUNT 100


/**
 * @brief Checks the stdin for failure
 *
 * If a failure is detected, stream is cleared of remaining character and its state.
 * Console is cleared and a message is outputted signaling wrong input.
 *
 * @return true input failed
 * @return false input correct
 */
bool checkInputForFail() {
    if (std::cin.fail()) {
        std::cin.clear(); // Clean stdin for next tries
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
        std::cout << "(!) Wrong input.\n";
        return true;
    }
    return false;
}

/**
 * @brief Load FIT CTU's semester from file
 *
 * User is prompted for a path containing parallels of courses from KOS FIT CTU (in Czech).
 * In case of a problem with loading, an error message is outputted to error stream
 * and the program is exited with failure.
 *
 * @param logo string containing ascii art logo
 * @return Semester imported semester
 */
Semester loadSemester(std::string & logo) {
    // Print header
    std::cout << logo << std::endl;
    std::cout << std::string(SEPARATOR_LENGTH, '=') << '\n';
    std::cout << "Enter a path to a file with parallels of courses from KOS FIT CTU (in Czech):\n";
    std::cout << std::string(SEPARATOR_LENGTH, '_') << std::endl;

    // Get input
    std::string filepath;
    std::cin >> filepath;

    // Process input
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

/**
 * @brief Load priorities from user.
 *
 * The user is prompted to adjust priorities.
 * If user declines, default priorities are used, else they are configured from
 * standard input.
 *
 * @param logo string containing ascii art logo
 * @param semester loaded semester, for which priorities will be set
 * @return Priorities set priorities
 */
Priorities loadPriorities(std::string & logo, Semester & semester) {
    Priorities result;
    char choice;
    do {
        // Print header
        std::cout << logo << std::endl;
        std::cout << std::string(SEPARATOR_LENGTH, '=') << '\n';
        std::cout << "Adjust priorities for schedule properties? (y/n):\n";
        std::cout << std::string(SEPARATOR_LENGTH, '_') << std::endl;

        // Get input
        std::cin >> choice;
        if (checkInputForFail()) {
            continue;
        }

        // Input validation
        if (choice == 'y' || choice == 'Y') {
            break;
        } else if (choice == 'n' || choice == 'N') { // Return default priorities
            return result;
        }
    } while (true);

    // Configure priorities
    StdinAdjuster adjuster;
    result = adjuster(semester);

    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    return result;
}

/**
 * @brief Generates a timetable and outputs result to standard output
 *
 * Function prompts the user to enter the number of generations to use.
 *
 * @param logo
 * @param semester Semester to generate timetable for
 * @param priorities Priorities to use
 */
void evolve(std::string & logo, Semester & semester, Priorities & priorities) {
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console

    // Create evolution and calculate generation size
    Evolution evolution(semester, priorities);
    size_t generationSize = evolution.getGenomeSize() * GENERATION_SIZE_MULTIPLIER;

    std::cin.ignore(); // Clear previous character stuck in cin

    // Retrieve generation count
    unsigned int generationCount = GENERATION_COUNT;
    do {
        // Print header
        std::cout << logo << std::endl;
        std::cout << std::string(SEPARATOR_LENGTH, '=') << '\n';
        std::cout << "Number of generations.\n\n";
        std::cout << "Lager generation count may improve properties of the generated timetable (if the ideal schedule has not been achieved),\n";
        std::cout << "but will greatly increase required computation time.\n";
        std::cout << "The default number is [" << GENERATION_COUNT << "] in most cases it is recommended to keep at default.\n";
        std::cout << "Enter generation count number (or press enter to keep default number):\n";
        std::cout << std::string(SEPARATOR_LENGTH, '_') << std::endl;

        // Get input
        std::string line;
        std::getline(std::cin, line);

        if (line.empty()) { // Use default value
            generationCount = GENERATION_COUNT;
            break;
        }

        // Input validation
        try {
            generationCount = std::stoul(line);
        }
        catch (...) {
            std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
            std::cout << "(!) Unable to read input.\n";
            continue;
        }

        if (generationCount == 0) {
            std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
            std::cout << "(!) Generaton count can not be zero.\n";
            continue;
        }

        break;
    } while (true);

    // Evolve
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    std::cout << logo << std::endl;
    std::cout << generationCount << " generations" << std::endl;
    std::vector<EvolutionResult> result = evolution.evolve(generationSize, generationCount);

    // Print output
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    std::cout << logo << std::endl;
    CS_StdoutOutputter outputter;
    outputter.output(result);
}

int main() {
    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console

    std::string logo; // Ascii art logo
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
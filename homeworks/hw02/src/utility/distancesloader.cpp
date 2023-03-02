#include "utility/distancesloader.h"

const char * LoaderException::what() const noexcept {
    return message.c_str();
}

Distances readDistances(const std::string & filePath) {

    std::string line;
    std::string word;
    std::vector<unsigned long> row;

    size_t cityCount; // Expected file size recived in first line
    bool setCityCount = false; // Signaling first row has been parsed
    std::vector<std::vector<unsigned long>> distances; // Vector of distances from file (rows)
    std::vector<std::string> cities; // Vector of cities in file (first column)

    std::fstream file;
    file.open(filePath, std::ios::in);

    if (!file.is_open()) {
        throw LoaderException("File could not be opened.");
    }

    while (std::getline(file, line)) { // Read each line
        row.clear();

        std::stringstream lineStream(line);

        bool first = true;
        while (std::getline(lineStream, word, ',')) { // Read each word (item separated by ',')
            if (first) { // Parse first column
                cities.push_back(word);
                first = false;
                continue;
            }

            unsigned long value = std::stoul(word); // Try to parse distance as value

            row.push_back(value);
        }

        if (!setCityCount) { // Set size based on first row
            cityCount = row.size();
            setCityCount = true;
        } else {
            if (cityCount != row.size()) { // Check if row has same size as other rows
                throw LoaderException("Wrong row length in file.");
            }
        }

        distances.push_back(row);
    }

    // Check if all sizes of objects are correct
    if ((distances.size() != cityCount)
        || cities.size() != cityCount) {
        throw LoaderException("Columns do not equal rows in file.");
    }

    return Distances(distances, cities);
}
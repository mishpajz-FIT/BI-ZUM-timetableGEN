#include "utility/distancesloader.h"

const char * LoaderException::what() const noexcept {
    return message.c_str();
}

Distances readDistances(const std::string & filePath) {

    std::string line;
    std::string word;
    std::vector<unsigned long> row;

    size_t cityCount;
    bool setCityCount = false;
    std::vector<std::vector<unsigned long>> distances;
    std::vector<std::string> cities;

    std::fstream file;
    file.open(filePath, std::ios::in);

    if (!file.is_open()) {
        throw LoaderException("File could not be opened.");
    }

    while (std::getline(file, line)) {
        row.clear();

        std::stringstream lineStream(line);

        bool first = true;
        while (std::getline(lineStream, word, ',')) {
            if (first) {
                cities.push_back(word);
                first = false;
                continue;
            }

            unsigned long value = std::stoul(word);

            row.push_back(value);
        }

        if (!setCityCount) {
            cityCount = row.size();
            setCityCount = true;
        } else {
            if (cityCount != row.size()) {
                throw LoaderException("Wrong row length in file.");
            }
        }

        distances.push_back(row);
    }

    if ((distances.size() != cityCount)
        || cities.size() != cityCount) {
        throw LoaderException("Columns do not equal rows in file.");
    }

    return Distances(distances, cities);
}
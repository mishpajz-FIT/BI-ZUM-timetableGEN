#include "tsphillclimbing.h"

Configuration::Configuration(size_t size): indexes() {
    for (size_t i = 0; i < size; i++) {
        indexes.push_back(i);
    }

    shuffleFisherYates();
}

void Configuration::shuffleFisherYates() {

    if (indexes.size() <= 1) {
        return;
    }

    for (size_t i = 0; i < indexes.size() - 1; i++) {
        size_t j = randomIndex(i, 0);
        std::swap(indexes[i], indexes[j]);
    }
}

std::vector<Configuration> Configuration::neighbours() const {
    Configuration newConfiguration(*this);

    size_t firstIndex = newConfiguration.indexes.front();
    newConfiguration.indexes.pop_front();
    newConfiguration.indexes.push_back(firstIndex);

    if (indexes.size() <= 1) {
        return { };
    }

    std::vector<Configuration> results;
    for (size_t i = 0; i < indexes.size() - 1; i++) {
        for (size_t j = i + 1; j < indexes.size(); j++) {
            Configuration result = newConfiguration;
            std::swap(result.indexes[i], result.indexes[j]);
            results.push_back(result);
        }
    }

    return results;
}

std::ostream & operator << (std::ostream & os, const Configuration & configuration) {

    os << "{ ";

    if (configuration.indexes.size() == 0) {
        os << " }";
        return os;
    }

    for (size_t i = 0; i < configuration.indexes.size(); i++) {

        os << configuration.indexes[i];

        if (i != configuration.indexes.size() - 1) {
            os << ", ";
        }
    }

    os << " }";
    return os;
}

size_t Configuration::randomIndex(size_t largerThan, size_t smallerThan) const {

    if (smallerThan == 0) {
        smallerThan = indexes.size() - 1;
    }

    if (largerThan > smallerThan) {
        throw std::out_of_range("Random index out of range.");
    }

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<size_t> distribution(largerThan, smallerThan);
    size_t randomIndex = distribution(generator);

    return randomIndex;
}

unsigned long totalDistance(const Configuration & forConfiguration, const Distances & distances) {
    unsigned long result = 0;

    if (forConfiguration.indexes.size() == 1) {
        return result;
    }

    for (size_t i = 0; i < forConfiguration.indexes.size() - 1; i++) {
        size_t current = forConfiguration.indexes[i];
        size_t next = forConfiguration.indexes[i + 1];

        result += distances.values[current][next];
    }

    size_t last = forConfiguration.indexes.back();
    size_t first = forConfiguration.indexes.front();
    result += distances.values[last][first];

    return result;
}

std::pair<Configuration, unsigned long> bestConfiguration(std::vector<Configuration> configurations, const Distances & distances, bool verbose) {

    std::vector<std::pair<Configuration, unsigned long>> withDistances;

    if (verbose) {
        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
        std::cout << "_configurations_:" << std::endl;
    }

    for (auto & conf : configurations) {
        unsigned long distance = totalDistance(conf, distances);
        withDistances.emplace_back(conf, distance);

        if (verbose) {
            std::cout << conf << ": " << distance << std::endl;
        }
    }

    auto comp = [ ](const std::pair<Configuration, unsigned long> & lhs, const std::pair<Configuration, unsigned long> & rhs) -> bool {
        return lhs.second < rhs.second;
    };
    std::sort(withDistances.begin(), withDistances.end(), comp);

    return withDistances[0];
}

void printSeparator(bool big) {

    if (big) {
        std::cout << "****************************************" << std::endl;
        return;
    }

    std::cout << "-------------------------" << std::endl;
}

std::pair<Configuration, unsigned long> hillclimbing(const Distances & distances, bool verbose) {

    Configuration initial(distances.cities.size());
    unsigned long initialDistance = totalDistance(initial, distances);
    std::pair<Configuration, unsigned long> current = std::make_pair(initial, initialDistance);
    std::pair<Configuration, unsigned long> best = current;

    if (verbose) {
        std::cout << initial << ": " << initialDistance << std::endl;
    }

    size_t i = 0;

    while (true) {
        if (verbose) {
            printSeparator();
        }

        std::pair<Configuration, unsigned long> next = bestConfiguration(current.first.neighbours(), distances, verbose);

        if (verbose) {
            std::cout << "_best_distance_: (" << current.second << ")" << std::endl;
            std::cout << "_steps_: (" << i++ << ")" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)(250)));
        }

        if (next.second >= current.second) {
            break;
        }

        current = next;
        best = current;
    }

    if (verbose) {
        printSeparator();
        std::cout << "_best_reached_distance_: (" << current.second << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)(1000)));
    }

    return current;
}

void tsp(const Distances & distances) {

    std::pair<Configuration, unsigned long> result = std::make_pair(0, 0);
    bool firstRun = true;

    size_t tries = 5;
    for (size_t i = 0; i < tries; i++) {
        printSeparator(true);
        std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
        std::cout << "TRY NO: (" << i + 1 << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)(1000)));
        std::pair<Configuration, unsigned long> run = hillclimbing(distances, true);
        if (firstRun) {
            firstRun = false;
            result = run;
            continue;
        }
        if (run.second < result.second) {
            result = run;
            tries++;
        }
    }

    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    printSeparator(true);
    std::cout << "RESULT: " << std::endl;
    std::cout << result.first << std::endl;
    std::cout << "distance = " << result.second << std::endl;
}

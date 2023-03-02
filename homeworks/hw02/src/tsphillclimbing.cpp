#include "tsphillclimbing.h"

Configuration::Configuration(size_t size): indexes() {
    for (size_t i = 0; i < size; i++) {
        indexes.push_back(i);
    }

    shuffleFisherYates();
}

std::vector<Configuration> Configuration::neighbours() const {
    Configuration newConfiguration(*this);

    if (indexes.size() <= 1) {
        return { };
    }

    std::vector<Configuration> results;
    for (size_t i = 0; i < indexes.size() - 1; i++) {
        for (size_t j = i + 1; j < indexes.size(); j++) { // Generate all configurations with two indexes swapped
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

    for (size_t i = 0; i < configuration.indexes.size(); i++) { // Print each index

        os << configuration.indexes[i];

        if (i != configuration.indexes.size() - 1) { // Separator between indexes (do not print on last index)
            os << ", ";
        }
    }

    os << " }";
    return os;
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

size_t Configuration::randomIndex(size_t largerThan, size_t smallerThan) const {

    if (smallerThan == 0) { // Special case (smallerThan should be indexes size if smallerThan == 0)
        smallerThan = indexes.size() - 1;
    }

    if (largerThan > smallerThan) { // Wrong arguments
        throw std::out_of_range("Random index out of range.");
    }

    // Random number generation
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

    for (size_t i = 0; i < forConfiguration.indexes.size() - 1; i++) { // Calculate distance between each two indexes
        size_t current = forConfiguration.indexes[i];
        size_t next = forConfiguration.indexes[i + 1];

        result += distances.values[current][next];
    }

    // Calculate  distance between first and last index (close the loop)
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

    for (auto & conf : configurations) { // For each configuration calculate distance
        unsigned long distance = totalDistance(conf, distances);
        withDistances.emplace_back(conf, distance);

        if (verbose) {
            std::cout << conf << ": " << distance << std::endl;
        }
    }

    // Sort configurations in ascending order based on distances
    auto comp = [ ](const std::pair<Configuration, unsigned long> & lhs, const std::pair<Configuration, unsigned long> & rhs) -> bool {
        return lhs.second < rhs.second;
    };
    std::sort(withDistances.begin(), withDistances.end(), comp);

    return withDistances[0]; // Return configuration with smallest distance
}

void printSeparator(bool big) {

    if (big) {
        std::cout << "****************************************" << std::endl;
        return;
    }

    std::cout << "-------------------------" << std::endl;
}

std::pair<Configuration, unsigned long> hillclimbing(const Distances & distances, bool verbose) {

    Configuration initial(distances.cities.size()); // Generate random initial configuration
    unsigned long initialDistance = totalDistance(initial, distances);

    if (verbose) {
        std::cout << initial << ": " << initialDistance << std::endl;
    }

    size_t i = 0;
    std::pair<Configuration, unsigned long> current = std::make_pair(initial, initialDistance); // Configuration iterator

    while (true) {
        if (verbose) {
            printSeparator();
        }

        // Get next iterator as neighbor with smallest distance
        std::pair<Configuration, unsigned long> next = bestConfiguration(current.first.neighbours(), distances, verbose);

        if (verbose) {
            std::cout << "_best_distance_: (" << current.second << ")" << std::endl;
            std::cout << "_steps_: (" << i++ << ")" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)(250)));
        }

        if (next.second >= current.second) { // If neighbour with smallest distance is not better, reached local minima
            if (verbose) {
                std::cout << "_local_minima_reached_" << std::endl;
            }
            break;
        }

        current = next;
    }

    if (verbose) {
        printSeparator();
        std::cout << "_best_reached_distance_: (" << current.second << ")" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)(1000)));
    }

    return current;
}

unsigned long tsp(const Distances & distances, bool verbose) {

    std::pair<Configuration, unsigned long> result = std::make_pair(0, 0);

    bool firstRun = true;
    size_t tries = distances.cities.size() / 3 + 3; // Calculate amount of restarts based on number of cities
    for (size_t i = 0; i < tries; i++) {
        if (verbose) {
            printSeparator(true);
            std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
            std::cout << "TRY NO: (" << i + 1 << ")" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)(1000)));
        }

        std::pair<Configuration, unsigned long> run = hillclimbing(distances, verbose); // Perform hill climbing
        if (firstRun) { // On first run store variables
            firstRun = false;
            result = run;
            continue;
        }

        if (run.second < result.second) { // If hill climbing produced better result than before, add another try
            result = run;
            tries++;
        }
    }

    std::cout << "\033[1;1H\033[2J" << std::endl; // Clean console
    printSeparator(true);
    std::cout << "RESULT: " << std::endl;
    std::cout << "{ ";
    for (size_t i = 0; i < result.first.indexes.size(); i++) { // Print names of cities that produced best configuration (with smallest distance)

        std::cout << distances.cities[i];

        if (i != result.first.indexes.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << " }" << std::endl;
    std::cout << "distance = " << result.second << std::endl;

    return result.second;
}

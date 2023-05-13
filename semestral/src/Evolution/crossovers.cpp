#include "Evolution/crossovers.h"

CrossoverException::CrossoverException(std::string message) : msg(std::move(message)) { }

const char * CrossoverException::what() const noexcept {
    return msg.c_str();
}

size_t Crossover::genomeCheck(const Genome & lParent, const Genome & rParent) {
    if (lParent.size() != rParent.size()) {
        throw CrossoverException("Crossover got parents with different genome lengths.");
    }

    return lParent.size();
}

RANDOM_CROSSOVER_NUMBER_TYPE Crossover::randomNumber() {
    static std::random_device device;
    static std::mt19937 rng(device());
    static std::uniform_int_distribution<RANDOM_CROSSOVER_NUMBER_TYPE> distribution(0, INT32_MAX);

    return distribution(rng);
}

Genome UniformCrossover::perform(const Genome & lParent, const Genome & rParent) {
    size_t genomeSize = genomeCheck(lParent, rParent);

    Genome descendant;

    size_t randomNumBit = 0;
    uint32_t randomNum = randomNumber();
    for (size_t i = 0; i < genomeSize; i++) {
        if (randomNumBit == RANDOM_CROSSOVER_NUMBER_SIZE) {
            randomNum = randomNumber();
            randomNumBit = 0;
        }

        unsigned short bit = (randomNum >>= 1) & 1;


        if (bit == 0) {
            descendant.push_back(lParent[i]);
        } else {
            descendant.push_back(rParent[i]);
        }

        randomNumBit++;
    }

    return descendant;
}

PointCrossover::PointCrossover(size_t k) : Crossover(), points(k) {
    if (points == 0) {
        throw CrossoverException("Imossible amount of crossover points.");
    }
}

Genome PointCrossover::perform(const Genome & lParent, const Genome & rParent) {
    size_t genomeSize = genomeCheck(lParent, rParent);

    if (points >= genomeSize) {
        throw CrossoverException("Length of genome too small for multi-point crossover.");
    }

    std::set<size_t> crossoverPoints;
    while (crossoverPoints.size() < points) {
        size_t point = randomNumber() % genomeSize;
        crossoverPoints.insert(point);
    }

    Genome descendant;

    bool parentParity = true;
    for (size_t i = 0; i < genomeSize; i++) {

        if (crossoverPoints.contains(i)) {
            parentParity = !parentParity;
        }

        if (parentParity) {
            descendant.push_back(lParent[i]);
        } else {
            descendant.push_back(rParent[i]);
        }
    }

    return descendant;
}
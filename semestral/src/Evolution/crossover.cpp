#include "Evolution/crossover.h"

#define randomNumberType uint32_t
#define randomNumberSize 32

Genome SimpleCrossover::perform(const Genome &lParent, const Genome &rParent) {

    if (lParent.size() != rParent.size()) {
        // TODO: Throw exception
    }

    size_t genomeSize = lParent.size();

    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<randomNumberType> distribution(0, INT32_MAX);

    Genome descendant;

    size_t randomNumberBit = 0;
    uint32_t randomNumber = distribution(rng);
    for (size_t i = 0; i < genomeSize; i++) {
        if (randomNumberBit == randomNumberSize) {
            randomNumber = distribution(rng);
            randomNumberBit = 0;
        }

        unsigned short bit = (randomNumber >>= 1) & 1;
        if (bit == 0) {
            descendant.push_back(lParent[i]);
        } else {
            descendant.push_back(rParent[i]);
        }
        randomNumberBit++;
    }

    return descendant;
}
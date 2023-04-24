#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <vector>
#include <utility>
#include <cstdint>
#include <random>

using Genome = std::vector<uint32_t>;

struct Crossover {

    virtual ~Crossover() = default;

    virtual Genome perform(const Genome & lParent, const Genome & rParent) = 0;
};

struct SimpleCrossover: Crossover {

    Genome perform(const Genome & lParent, const Genome & rParent) override;
};

#endif /* CROSSOVER_H */
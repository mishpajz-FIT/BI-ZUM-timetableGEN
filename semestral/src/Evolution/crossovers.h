#ifndef CROSSOVERS_H
#define CROSSOVERS_H

#include <vector>
#include <utility>
#include <cstdint>
#include <random>
#include <exception>
#include <set>

#define RANDOM_CROSSOVER_NUMBER_TYPE uint32_t
#define RANDOM_CROSSOVER_NUMBER_SIZE 32

using Genome = std::vector<uint32_t>;

struct CrossoverException : public std::exception {

    CrossoverException(std::string message);

    const char * what() const noexcept override;

private:

    std::string msg;
};

struct Crossover {

    virtual ~Crossover() = default;

    virtual Genome perform(const Genome & lParent, const Genome & rParent) = 0;

protected:

    size_t genomeCheck(const Genome & lParent, const Genome & rParent);

    static RANDOM_CROSSOVER_NUMBER_TYPE randomNumber();
};

struct UniformCrossover : Crossover {

    Genome perform(const Genome & lParent, const Genome & rParent) override;
};

struct PointCrossover : Crossover {
private:
    size_t points;

public:
    PointCrossover(size_t k);

    Genome perform(const Genome & lParent, const Genome & rParent) override;
};

#endif /* CROSSOVERS_H */
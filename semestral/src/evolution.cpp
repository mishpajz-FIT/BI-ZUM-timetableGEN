#include "evolution.h"

Evolution::Evolution(const std::vector<Course> & c):
        courses(c),
        genomeIndexToCourse(),
        genomeIndexToEntry(),
        maxValueGenome(),
        currentGeneration(),
        generationNumber(0) {


    size_t courIndex = 0;
    for (auto & cour: courses) {
        size_t claIndex = 0;

        for (auto & entr: cour.entries) {
            maxValueGenome.push_back(entr.schedules.size());

            genomeIndexToCourse.push_back(courIndex);
            genomeIndexToEntry.push_back(claIndex);

            claIndex++;
        }

        courIndex++;
    }

    crossovers.emplace_back(new SimpleCrossover());
}

std::vector<std::map<Course, std::map<std::string, uint32_t>>>
Evolution::evolve(size_t generationSize, size_t maxGenerations) {

    createInitialGenerations(generationSize);

    size_t genomeSize = maxValueGenome.size();

    for (size_t gen = 0; gen < maxGenerations; gen++) {
        // TODO: evolve


        std::vector<Genome> newGeneration;
        for (size_t child = 0; child < generationSize; child++)
            size_t lParentIndex = randomNumber(genomeSize);
            size_t rParentIndex = randomNumber(genomeSize);


            Genome child = crossovers[0]->perform(currentGeneration[lParentIndex], currentGeneration[rParentIndex]);

            newGeneration.push_back(child);
        }

        std::swap(currentGeneration, newGeneration);
    }
}

void Evolution::createInitialGenerations(size_t generationSize) {
    currentGeneration.clear();

    size_t genomeSize = maxValueGenome.size();

    for (size_t i = 0; i < generationSize; i++) {
        Genome newGenome;

        for (size_t j = 0; j < genomeSize; j++) {
            newGenome.push_back(randomNumber(maxValueGenome[j]));
        }
    }
}

size_t Evolution::randomNumber(size_t maxValue) {

    static std::random_device device;
    static std::mt19937 rng(device());

    std::uniform_int_distribution<size_t> distribution(0, maxValue);

    return distribution(rng);
}


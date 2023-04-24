#include "evolution.h"

Evolution::Evolution(const std::vector<Course> & c):
    courses(c),
    genomeIndexToCourse(),
    genomeIndexToClass(),
    maxValueGenome(),
    currentGeneration(),
    generationNumber(0) {


    size_t courIndex = 0;
    for (auto & cour: courses) {
        size_t claIndex = 0;

        for (auto & cla: subj.classes) {
            maxValueGenome.push_back(cla.entries.size());

            genomeIndexToCourse.push_back(courIndex);
            genomeIndexToClass.push_back(claIndex);

            claIndex++;
        }

        courIndex++;
    }
}

std::vector<std::map<Course, std::map<std::string, uint32_t>>>
Evolution::evolve(size_t generationSize, size_t maxGenerations) {

    createInitialGenerations(generationSize);

    generationNumber = 0;

    for (size_t i = 0; i < maxGenerations; i++) {
        // TODO: evolve
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


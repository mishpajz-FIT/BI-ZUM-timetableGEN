#include "evolution.h"

Evolution::Evolution(const Semester & s, const Priorities & p) :
    semester(s),
    priorities(p),
    genomeSize(0),
    genomeIndexToSchedule(),
    courseAndScheduleToGenomeIndex(),
    crossovers(),
    currentGeneration() {

    size_t i = 0;
    for (auto & coursePtr : s.coursesPtrs) {
        for (auto & schedulePtr : coursePtr->schedulesPtrs) {
            genomeIndexToSchedule.push_back(schedulePtr.second);
            courseAndScheduleToGenomeIndex[std::make_pair(coursePtr->name, schedulePtr.first)] = i;

            i++;
        }
    }
    genomeSize = i;

    crossovers.emplace_back(new SimpleCrossover());
}

std::vector<std::map<Course, std::map<std::string, uint32_t>>>
Evolution::evolve(size_t generationSize, size_t maxGenerations) {

    createInitialGenerations(generationSize);

    for (size_t gen = 0; gen < maxGenerations; gen++) {
        // TODO: evolve


        std::vector<Genome> newGeneration;
        for (size_t childIndex = 0; childIndex < generationSize; childIndex++) {
            size_t lParentIndex = randomNumber(genomeSize);
            size_t rParentIndex = randomNumber(genomeSize);

            Genome child = crossovers[0]->perform(currentGeneration[lParentIndex], currentGeneration[rParentIndex]);

            newGeneration.push_back(child);
        }
        std::swap(currentGeneration, newGeneration);
    }

    return std::vector<std::map<Course, std::map<std::string, uint32_t>>>();

}

double Evolution::fitness(const Genome & genome) {

    using IntervalEntry = std::pair<TimeInterval, std::shared_ptr<Entry>>;
    std::vector<IntervalEntry> intervals;

    size_t genomeIndex = 0;
    for (auto entryIndex : genome) {
        std::shared_ptr<Entry> entry = genomeIndexToSchedule[genomeIndex]->entriesPtrs[entryIndex];

        for (auto & interval : entry->timeslots) {
            intervals.push_back(std::make_pair(interval, entry));
        }

        genomeIndex++;
    }

    std::sort(intervals.begin(), intervals.end(), [ ] (const IntervalEntry & lhs, const IntervalEntry & rhs) -> bool {
        return lhs.first < rhs.first;
        });

    /*

    for (auto it = intervals.begin(); it != intervals.end(); it++) {
        auto interIt = it + 1;
        while (interIt != intervals.end()
            && ((interIt->first.startTime < it->first.endTime) && (it->first.day == interIt->first.day))) {

            if (it->first.parity == interIt->first.parity
                || (it->first.parity == TimeInterval::Parity::Both || interIt->first.parity == TimeInterval::Parity::Both)) {

                it->second->collisions.insert(interIt->second);
                interIt->second->collisions.insert(it->second);
            }

            interIt++;
        }

    }


    */


}

void Evolution::createInitialGenerations(size_t generationSize) {
    currentGeneration.clear();

    for (size_t i = 0; i < generationSize; i++) {
        Genome newGenome;

        for (size_t j = 0; j < genomeSize; j++) {
            size_t maxValue = genomeIndexToSchedule[j]->entriesPtrs.size();
            newGenome.push_back(randomNumber(maxValue));
        }
    }
}

size_t Evolution::randomNumber(size_t maxValue) {

    static std::random_device device;
    static std::mt19937 rng(device());

    std::uniform_int_distribution<size_t> distribution(0, maxValue);

    return distribution(rng);
}


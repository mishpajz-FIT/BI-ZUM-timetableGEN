#include "evolution.h"

Evolution::Evolution(const Semester & s, const Priorities & p):
    semester(s),
    priorities(p),
    genomeSize(0),
    genomeIndexToSchedule(),
    courseAndScheduleToGenomeIndex(),
    crossovers() {

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

std::vector<std::pair<EntryAddress, std::shared_ptr<Entry>>>
Evolution::evolve(size_t generationSize, size_t maxGenerations, bool verbal) {

    std::vector<Genome> currentGeneration;

    currentGeneration = createInitialGenerations(generationSize);

    if (verbal) {
        std::cout << "Evolution generation:" << std::endl;
    }

    for (size_t gen = 0; gen < maxGenerations; gen++) {
        // TODO: Evolve

        if (verbal) {
            std::cout << "  |- " << gen + 1 << std::endl;
        }

        std::vector<Genome> newGeneration;
        while (newGeneration.size() < generationSize * generationSize) {
            size_t lParentIndex = randomNumber(genomeSize);
            size_t rParentIndex = randomNumber(genomeSize);

            Genome child = crossovers[0]->perform(currentGeneration[lParentIndex], currentGeneration[rParentIndex]);

            // TODO: Mutations

            newGeneration.emplace_back(child);
        }

        // Add elite of current generation to new generation
        size_t eliteSize = generationSize / 10;
        for (size_t i = 0; i < eliteSize; i++) {
            newGeneration.push_back(currentGeneration[i]);
        }

        selection(newGeneration, generationSize);
        currentGeneration = newGeneration;
    }

    Genome best = currentGeneration.front();
    std::vector<std::pair<EntryAddress, std::shared_ptr<Entry>>> result;
    for (size_t i = 0; i < genomeSize; i++) {
        std::shared_ptr<Schedule> schedule = genomeIndexToSchedule[i];
        EntryAddress address = std::make_pair(schedule->course.lock()->name, schedule->name);

        result.emplace_back(std::make_pair(address, schedule->entriesPtrs[best[i]]));
    }

    return result;
}

void Evolution::selection(std::vector<Genome> & newGeneration, size_t generationSize) {

    Scores minValues;
    Scores maxValues;

    std::vector<std::pair<Genome, Scores>> scoredGenomes;
    for (auto it = newGeneration.begin(); it != newGeneration.end(); it++) {
        Scores itScore = score(*it);
        scoredGenomes.emplace_back(std::make_pair(*it, itScore));
        minValues.setToMinValuesFrom(itScore);
        maxValues.setToMaxValuesFrom(itScore);
    }

    using GenomeFitness = std::pair<Genome, double>;
    std::vector<GenomeFitness> fitnessedGenomes;
    for (auto it = scoredGenomes.begin(); it != scoredGenomes.end(); it++) {
        fitnessedGenomes.emplace_back(std::make_pair(it->first, fitness(it->second, minValues, maxValues)));
    }

    std::sort(fitnessedGenomes.begin(), fitnessedGenomes.end(), [ ] (const GenomeFitness & lhs, const GenomeFitness & rhs) -> bool {
        return lhs.second > rhs.second;
        });


    std::vector<Genome> result;
    if (generationSize > fitnessedGenomes.size()) {
        generationSize = fitnessedGenomes.size();
    }

    for (size_t i = 0; i < generationSize; i++) {
        result.emplace_back(fitnessedGenomes[i].first);
    }

    std::swap(newGeneration, result);
}

double Evolution::fitness(const Scores & genomeScore, const Scores & minValues, const Scores & maxValues) {
    double result = 0;

#define inverseScoring(val) (10 - 9 * ((genomeScore.val - minValues.val) / (maxValues.val - minValues.val)));

    result += inverseScoring(coherentInDay);
    result += inverseScoring(coherentInWeek);
    result += inverseScoring(collisions);

    return result;

    // TODO: Rest of the coherences
}


Scores Evolution::score(const Genome & genome) {

    // Get all intervals and sort them by start time
    using IntervalEntry = std::pair<TimeInterval, std::shared_ptr<Entry>>;
    std::vector<IntervalEntry> intervals;

    for (size_t genomeIndex = 0; genomeIndex < genomeSize; genomeIndex++) {
        std::shared_ptr<Entry> entry = genomeIndexToSchedule[genomeIndex]->entriesPtrs[genome[genomeIndex]];
        for (auto & interval : entry->timeslots) {
            intervals.push_back(std::make_pair(interval, entry));
        }
    }


    std::sort(intervals.begin(), intervals.end(), [ ] (const IntervalEntry & lhs, const IntervalEntry & rhs) -> bool {
        return lhs.first < rhs.first;
        });


    Scores result;


    for (auto it = intervals.begin(); it != intervals.end(); it++) {

        // Day coherence score
        auto next = it + 1;
        if (next != intervals.end() && it->first.day == next->first.day) {
            result.coherentInDay += (next->first.startTime.valueInMinutes() - it->first.endTime.valueInMinutes());
        }


        // Collision score
        auto collisionIt = it + 1;
        while (collisionIt != intervals.end()
            && ((collisionIt->first.startTime < it->first.endTime) && (it->first.day == collisionIt->first.day))) {

            if (it->first.parity == collisionIt->first.parity
                || (it->first.parity == TimeInterval::Parity::Both || collisionIt->first.parity == TimeInterval::Parity::Both)) {

                result.collisions++;
            }

            collisionIt++;
        }

    }

    // Week coherence score
    result.coherentInWeek = static_cast<size_t>(intervals.back().first.day) - static_cast<size_t>(intervals.front().first.day);

    // TODO: Rest of the coherences

    return result;
}

std::vector<Genome> Evolution::createInitialGenerations(size_t generationSize) const {

    std::vector<Genome> result;

    for (size_t i = 0; i < generationSize; i++) {
        Genome newGenome;

        for (size_t j = 0; j < genomeSize; j++) {
            size_t maxValue = genomeIndexToSchedule[j]->entriesPtrs.size();
            newGenome.push_back(randomNumber(maxValue));
        }

        result.push_back(newGenome);
    }

    return result;
}

size_t Evolution::randomNumber(size_t maxValue) {

    static std::random_device device;
    static std::mt19937 rng(device());

    if (maxValue != 0) {
        maxValue--;
    }

    std::uniform_int_distribution<size_t> distribution(0, maxValue);

    return distribution(rng);
}


#include "evolution.h"

#define EVOLUTION_PROGRESS_BAR_WIDTH 50

#define EVOLUTION_POINT_CROSSOVER_DIVIDER 10
#define EVOLUTION_MUTATION_ONE_IN 2
#define EVOLUTION_MUTATION_DIVIDER 25

Evolution::Evolution(const Semester & s, const Priorities & p) :
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

    crossovers.emplace_back(new UniformCrossover());
    crossovers.emplace_back(new PointCrossover(1));
    for (size_t i = 2; i <= (genomeSize / EVOLUTION_POINT_CROSSOVER_DIVIDER); i++) {
        crossovers.emplace_back(new PointCrossover(i));
    }
}

std::vector<EvolutionResult> Evolution::evolve(size_t generationSize, size_t maxGenerations, bool verbal) {

    std::vector<Genome> currentGeneration;

    currentGeneration = createInitialGenerations(generationSize);

    if (verbal) {
        std::cout << "Evolution:" << std::endl;
    }

    for (size_t gen = 0; gen < maxGenerations; gen++) {

        if (verbal) {
            loadingBar(std::cout, gen, maxGenerations);
        }

        std::vector<Genome> newGeneration;
        while (newGeneration.size() < generationSize * generationSize) {
            size_t lParentIndex = randomNumber(generationSize);
            size_t rParentIndex = randomNumber(generationSize);


            size_t crossoverIndex = randomNumber(crossovers.size());
            Crossover * crossover = crossovers[crossoverIndex].get();
            Genome child = crossover->perform(currentGeneration[lParentIndex], currentGeneration[rParentIndex]);


            mutate(child);
            for (size_t i = 1; i <= (genomeSize / EVOLUTION_MUTATION_DIVIDER); i++) {
                mutate(child);
            }

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

size_t Evolution::getGenomeSize() const {
    return genomeSize;
}

void Evolution::selection(std::vector<Genome> & newGeneration, size_t generationSize) {

    Scores minValues;
    Scores maxValues;

    bool first = true;
    std::vector<std::pair<Genome, Scores>> scoredGenomes;
    for (auto it = newGeneration.begin(); it != newGeneration.end(); it++) {
        Scores itScore = score(*it);
        scoredGenomes.emplace_back(std::make_pair(*it, itScore));

        if (first) {
            minValues = itScore;
            maxValues = itScore;
            first = false;
            continue;
        }

        minValues.setToMinValuesFrom(itScore);
        maxValues.setToMaxValuesFrom(itScore);
    }

    using GenomeFitness = std::pair<Genome, double>;
    std::vector<GenomeFitness> fitnessedGenomes;
    for (auto it = scoredGenomes.begin(); it != scoredGenomes.end(); it++) {
        fitnessedGenomes.emplace_back(std::make_pair(it->first, it->second.convertScoreToFitness(minValues, maxValues, priorities)));
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
    result.calculateScore(intervals, priorities);
    return result;
}

bool Evolution::mutate(Genome & genome) {
    size_t flag = randomNumber(EVOLUTION_MUTATION_ONE_IN);
    if (flag != 0) {
        return false;
    }

    size_t randomIndex = randomNumber(genomeSize);
    size_t maxValueOnIndex = genomeIndexToSchedule[randomIndex]->entriesPtrs.size();
    genome[randomIndex] = randomNumber(maxValueOnIndex);
    return true;
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

void Evolution::loadingBar(std::ostream & stream, size_t value, size_t maxValue) const {
    size_t percentage = (value * 100) / maxValue;
    size_t position = (EVOLUTION_PROGRESS_BAR_WIDTH * percentage) / 100;

    stream << "[";
    for (size_t i = 0; i < EVOLUTION_PROGRESS_BAR_WIDTH; i++) {
        if (i < position) {
            stream << "#";
        } else {
            stream << " ";
        }
    }
    stream << "] " << percentage << "%\r";
    stream.flush();
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


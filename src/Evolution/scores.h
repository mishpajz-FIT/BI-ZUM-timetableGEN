/**
 * @file scores.h
 * @author Michal Dobes
 * @date 2023-08-13
 *
 * @brief Data structures and calcualtions describing scores of priorities
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef SCORES_H
#define SCORES_H

#include "Data/priorities.h"

#include <vector>

struct Score {
    double value;

    Score();

    virtual ~Score() = default;

    virtual double getWeight() const = 0;

    /**
     * @brief Calculate this score's value
     *
     * Supplied intervals need to be sorted by start time.
     *
     * A superclass method should be called first.
     *
     * @see Scores.bonuses
     *
     * @param sortedIntervals intervals and it's entry
     * @param p priorities for timetable generation
     */
    virtual void calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p);

    /**
     * @brief Clone this score
     *
     * @return Score* cloned score
     */
    virtual Score * clone() const = 0;
};

/**
 * @brief Scores for selected Entries
 *
 * Fitness value is calculated based on these scores.
 *
 */
struct Scores {
private:
    Priorities priorities;

public:
    std::map<std::string, std::shared_ptr<Score>> scores; //!< All scores

    Scores() = delete;

    ~Scores();

    /**
     * @brief Construct a new Scores object
     *
     * @param s Scores to copy from
     */
    Scores(const Scores & s);

    /**
     * @brief Assignment operator
     *
     * @param s score to assign from
     * @return Scores& this
     */
    Scores & operator=(const Scores & s);

    /**
     * @brief Construct a new Scores object
     *
     * @param p priorities to base the scores on
     */
    Scores(const Priorities & p);

    /**
     * @brief Sets the scores to minimum value from another score
     *
     * Values are minimums from this and another score object.
     *
     * @param s another score
     * @return Scores& this
     *
     * @throws std::out_of_range the score types are not the same
     */
    Scores & setToMinValuesFrom(const Scores & s);

    /**
     * @brief Sets the scores to maximum value from another score
     *
     * Values are maximums from this and another score object.
     *
     * @param s another score
     * @return Scores& this
     *
     * @throws std::out_of_range the score types are not the same
     */
    Scores & setToMaxValuesFrom(const Scores & s);

    /**
     * @brief Calculate all scores for selected Entries
     *
     * @param intervals intervals and it's entry
     * @param p priorities for timetable generation
     */
    void calculateScore(std::vector<IntervalEntry> & intervals);

    /**
     * @brief Calculate fitness out of scores
     *
     * Requires minimum and maximum scores reached in this generation
     *
     * @param minValues minimum scores reached in this generation
     * @param maxValues maximum scores reached in this generation
     * @param p priorities for timetable generation
     * @return double fitness
     *
     * @throws std::out_of_range the score types are not the same
     */
    double convertScoreToFitness(const Scores & minValues, const Scores & maxValues) const;

private:

    /**
     * @brief Map value to inverse 0-10 scale
     *
     * Values are mapped in inverse to 0-10 scale, with the spacing between them kept proportionate.
     *
     * Largest value is mapped to smallest (0), smallest value is mapped to largest (10).
     *
     * @throws std::invalid_argument value is larger than max or smaller than min
     *
     * @param value value to map
     * @param min minimum value
     * @param max maximum value
     * @return double mapped value
     */
    inline double inverseScoring(double value, double min, double max) const;

};

/**
 * @brief Score for collisions
 *
 * Based on amount of collisions
 *
 */
struct CollisionsScore : public Score {

    double getWeight() const override;

    void calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) override;

    CollisionsScore * clone() const override;
};

/**
 * @brief Score for coherent in day
 *
 * Based on amount of incoherent parts in days
 *
 */
struct CoherentInDayScore : public Score {

    double getWeight() const override;

    void calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) override;

    CoherentInDayScore * clone() const override;
};

/**
 * @brief Score for coherent in week
 *
 * Based on length of week (from first entry to last)
 *
 */
struct CoherentInWeekScore : public Score {

    double getWeight() const override;

    void calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) override;

    CoherentInWeekScore * clone() const override;
};

/**
 * @brief Score for many consecutive hours
 *
 * Based on number of minutes over the preferred limit of consecutive hours
 *
 */
struct ManyConsecutiveHoursScore : public Score {

    double getWeight() const override;

    void calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) override;

    ManyConsecutiveHoursScore * clone() const override;
};

/**
 * @brief Score for wrong start times
 *
 * Based on minutes for start times after or before preferred limits
 *
 */
struct WrongStartTimesScore : public Score {

    double getWeight() const override;

    void calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) override;

    WrongStartTimesScore * clone() const override;
};

/**
 * @brief Score for bonuses
 *
 * Based on sum of bonuses in selected Entries
 *
 */
struct BonusesScore : public Score {

    double getWeight() const override;

    void calculateScore(std::vector<IntervalEntry> & sortedIntervals, const Priorities & p) override;

    BonusesScore * clone() const override;
};

#endif /* SCORES_H */
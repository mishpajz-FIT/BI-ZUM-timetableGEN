/**
 * @file adjuster.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Adjuster of properties
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ADJUSTER_H
#define ADJUSTER_H

#include "Evolution/priorities.h"
#include "Data/subjects.h"

/**
 * @brief Adjuster of properties
 *
 * Used for adjusting properties of timetable generation. Should be subclassed for specific source.
 *
 */
class Adjuster {
public:
    /**
     * @brief Retrieve adjusted properties from source
     *
     * @param semester semester the properties can adjust for
     * @return Priorities adjusted properties
     */
    virtual Priorities operator ()(Semester & semester) = 0;
};

#endif /* ADJUSTER_H */
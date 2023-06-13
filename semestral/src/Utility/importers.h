/**
 * @file importers.h
 * @author Michal Dobes
 * @date 2023-05-14
 *
 * @brief Importer of semester
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef IMPORTERS_H
#define IMPORTERS_H

#include "Data/subjects.h"

#include <vector>
#include <exception>
#include <string>
#include <algorithm>

/**
 * @brief Exception thrown by importer
 *
 */
struct ImporterException : public std::exception {

    ImporterException(std::string message);

    const char * what() const noexcept override;

private:

    std::string msg;
};

/**
 * @brief Importer of semester
 *
 * Used for importing a semester. Should be subclassed for specific source.
 *
 */
class Importer {
protected:
    /**
     * @brief Load semester from source
     *
     * @return Semester loaded semester
     */
    virtual Semester load() = 0;

public:
    Importer() = default;

    virtual ~Importer() = default;

    /**
     * @brief Import semester
     *
     * @see load()
     *
     * @return Semester imported semester
     */
    Semester import();

};

#endif /* IMPORTERS_H */

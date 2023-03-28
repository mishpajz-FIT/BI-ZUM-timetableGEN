#ifndef DISTANCESLOADER_H
#define DISTANCESLOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "distances.h"

/**
 * @brief Exception when parsing distances from file
 *
 */
class LoaderException: public std::exception {
protected:
    const std::string message;

public:
    LoaderException(const std::string & reason): message(reason) { }

    const char * what() const noexcept override;

};

/**
 * @brief Parse Distances file
 *
 * File needs to be in csv format
 * Matrix-like structure (first column name of city, all other column distances
 * to the cities in the same order as are rows in the file)
 *
 * @param filePath Path to the csv file with distances
 * @return Distances Parsed distances
 *
 * @throws LoaderException Wrong columns
 * @throws LoaderException Wrong rows
 * @throws LoaderException File could not be opened
 * @throws out_of_bounds Distance in wrong format (not unsigned long), could not be parsed
 */
Distances readDistances(const std::string & filePath);

#endif /* DISTANCESLOADER_H */
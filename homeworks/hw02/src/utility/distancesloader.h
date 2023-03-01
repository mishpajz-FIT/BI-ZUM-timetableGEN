#ifndef DISTANCESLOADER_H
#define DISTANCESLOADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "distances.h"

class LoaderException: public std::exception {
protected:
    const std::string message;

public:
    LoaderException(const std::string & reason): message(reason) { }

    const char * what() const noexcept override;

};

Distances readDistances(const std::string & filePath);

#endif /* DISTANCESLOADER_H */
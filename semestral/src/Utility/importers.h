#ifndef IMPORTERS_H
#define IMPORTERS_H

#include "Data/subjects.h"

#include <vector>
#include <exception>
#include <string>
#include <algorithm>

struct ImporterException : public std::exception {

    ImporterException(std::string message);

    const char * what() const noexcept override;

private:

    std::string msg;
};

class Importer {

    void calculateCollisions(Semester & semester);

protected:
    virtual Semester load() = 0;

public:
    Importer() = default;

    virtual ~Importer() = default;

    Semester import();

};

#endif /* IMPORTERS_H */

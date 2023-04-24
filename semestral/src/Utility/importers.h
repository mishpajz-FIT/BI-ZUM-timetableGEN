#ifndef IMPORTERS_H
#define IMPORTERS_H

#include "Data/subjects.h"

#include <vector>
#include <exception>
#include <string>


struct ImporterException : public std::exception {

    ImporterException(std::string message);

    const char * what() const noexcept override;

private:

    std::string msg;
};

class Importer {

public:
    Importer() = default;

    virtual ~Importer() = default;

    std::vector<Course> virtual import() = 0;

};


#endif /* IMPORTERS_H */

#ifndef FITCTUIMPORTER_H
#define FITCTUIMPORTER_H

#include "Utility/importers.h"
#include "Extensions/string_extensions.h"

#include <fstream>
#include <regex>

class FITCTUImporter: public Importer{

    std::ifstream file;

    FITCTUImporter(const std::string & filename);

    std::vector<Course> import() override;

private:

    enum class ReadingStates {
        Course,
        Entry,
        Capacity,
        TimeInterval,
        Parity,
        Lecturer,
        Location
    };

};


#endif /* FITCTUIMPORTER_H */

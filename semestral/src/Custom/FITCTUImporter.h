#ifndef FITCTUIMPORTER_H
#define FITCTUIMPORTER_H

#include "Utility/importers.h"
#include "Extensions/string_extensions.h"
#include "timeinterval.h"

#include <fstream>
#include <regex>
#include <map>

class FITCTUImporter: public Importer {

    std::ifstream file;

    FITCTUImporter(const std::string & filename);

    std::vector<Course> import() override;

protected:

    enum class ReadingStates {
        Course,
        Id,
        Type,
        Capacity,
        Time,
        Lecturer,
        Location
    };

    std::map<std::string, TimeInterval::Day> dayMapping;
    std::regex capacityRegex;
    std::regex timeRegex;
    std::map<std::string, TimeInterval::Parity> parityMapping;

};


#endif /* FITCTUIMPORTER_H */

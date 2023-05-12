#ifndef FITCTUFILEIMPORTER_H
#define FITCTUFILEIMPORTER_H

#include "Utility/importers.h"
#include "Extensions/string_extensions.h"
#include "Data/timeinterval.h"

#include <fstream>
#include <regex>
#include <map>

class FITCTUFileImporter: public Importer {

    std::ifstream file;

public:

    FITCTUFileImporter(const std::string & filename);

protected:

    Semester load() override;

    enum class ReadingStates {
        Course,
        Id,
        Type,
        Capacity,
        Time,
        Additional,
    };

    std::map<std::string, TimeInterval::Day> dayMapping;
    std::regex capacityRegex;
    std::regex timeRegex;
    std::map<std::string, TimeInterval::Parity> parityMapping;

};

class CS_FITCTUFileImporter: public FITCTUFileImporter {
public:
    CS_FITCTUFileImporter(const std::string & filename);
};

#endif /* FITCTUFILEIMPORTER_H */
